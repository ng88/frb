from __future__ import annotations

import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List, Optional, Set, Union

from bytecode import BinaryOp as CPBinaryOp, Compare

from src2.ast import nodes


@dataclass
class BytecodeArtifacts:
    source: Path
    output: Path
    language: str


PY311_PLUS = sys.version_info >= (3, 11)
PY312_PLUS = sys.version_info >= (3, 12)
CALL_OPCODE = "CALL" if PY311_PLUS else "CALL_FUNCTION"


class _NoArg:
    pass


_NO_ARG = _NoArg()


@dataclass(frozen=True)
class InstructionSpec:
    opname: str
    arg: Any = _NO_ARG


@dataclass(frozen=True)
class LabelSpec:
    name: str


@dataclass(frozen=True)
class LabelRef:
    name: str


InstructionEntry = Union[InstructionSpec, LabelSpec]


@dataclass
class CompiledRoutine:
    var_name: str
    method_name: str
    owner: Optional[str]
    shared: bool
    argnames: List[str]
    instructions: List[InstructionEntry]

    @property
    def qualname(self) -> str:
        if self.owner:
            return f"{self.owner}.{self.method_name}"
        return self.method_name


@dataclass
class CompiledProgram:
    routines: List[CompiledRoutine]
    class_methods: Dict[str, List[CompiledRoutine]]
    free_functions: List[CompiledRoutine]
    global_refs: Set[str]

    def has_main(self) -> bool:
        return any(r.owner == "Main" and r.method_name == "main" for r in self.routines)


class PythonBytecodeGenerator:
    def emit(self, program: nodes.Program, artifacts: BytecodeArtifacts) -> None:
        compiler = ProgramCompiler()
        compiled = compiler.compile_program(program)
        ensure(compiled.has_main(), "Program must define Main.main().")
        renderer = ModuleRenderer(compiled, artifacts)
        module_source = renderer.render()
        artifacts.output.parent.mkdir(parents=True, exist_ok=True)
        artifacts.output.write_text(module_source, encoding="utf-8")


class ProgramCompiler:
    def __init__(self) -> None:
        self._routines: List[CompiledRoutine] = []
        self._class_methods: Dict[str, List[CompiledRoutine]] = {}
        self._free_functions: List[CompiledRoutine] = []
        self._global_refs: Set[str] = set()
        self._used_names: Set[str] = set()

    def compile_program(self, program: nodes.Program) -> CompiledProgram:
        for decl in program.declarations:
            self._compile_class(decl)
        return CompiledProgram(
            routines=list(self._routines),
            class_methods=self._class_methods,
            free_functions=list(self._free_functions),
            global_refs=set(self._global_refs),
        )

    def _compile_class(self, cls: nodes.ClassDecl) -> None:
        ensure(isinstance(cls, nodes.ClassDecl), "Only class declarations are supported.")
        ensure(cls.class_kind is nodes.ClassKind.CLASS, "Only regular classes are supported.")
        ensure(not cls.template_params, "Template classes are not supported.")
        ensure(not cls.inherits, "Inheritance is not supported.")
        ensure(cls.name, "Class must have a name.")

        for member in cls.members:
            if isinstance(member, nodes.MethodDecl):
                routine = self._compile_method(cls, member)
                self._routines.append(routine)
                self._class_methods.setdefault(cls.name, []).append(routine)
            elif isinstance(member, nodes.FieldDecl):
                ensure(member.initializer is None, "Field initializers are not supported.")
            else:
                raise UnsupportedFeatureError(f"Unsupported class member {type(member).__name__}.")

    def _compile_method(self, cls: nodes.ClassDecl, method: nodes.MethodDecl) -> CompiledRoutine:
        ensure(isinstance(method.body, nodes.Block), "Method body must be a block.")
        compiler = MethodCompiler(method, owner=cls.name, global_refs=self._global_refs)
        instructions, argnames = compiler.compile()
        var_name = self._unique_var_name(cls.name, method.name)
        return CompiledRoutine(
            var_name=var_name,
            method_name=method.name,
            owner=cls.name,
            shared=method.shared,
            argnames=argnames,
            instructions=instructions,
        )

    def _unique_var_name(self, owner: Optional[str], name: str) -> str:
        base = f"{owner}_{name}" if owner else name
        safe = base.replace(".", "_")
        candidate = safe
        counter = 1
        while candidate in self._used_names:
            counter += 1
            candidate = f"{safe}_{counter}"
        self._used_names.add(candidate)
        return candidate


class MethodCompiler:
    def __init__(self, method: nodes.MethodDecl, owner: Optional[str], global_refs: Set[str]) -> None:
        self.method = method
        self.owner = owner
        self.global_refs = global_refs
        self.instructions: List[InstructionEntry] = []
        self.locals: Set[str] = set()
        self.argnames: List[str] = []
        self._temp_counter = 0
        self._label_counter = 0
        self._declare_parameters()

    def compile(self) -> tuple[List[InstructionEntry], List[str]]:
        self._lower_block(self.method.body)
        self._ensure_final_return()
        return self.instructions, self.argnames

    # ------------------------------------------------------------------
    def _declare_parameters(self) -> None:
        if not self.method.shared:
            self._add_argument("self")
        for param in self.method.parameters:
            ensure(param.name, "Parameter must have a name.")
            self._add_argument(param.name.lower())

    def _add_argument(self, name: str) -> None:
        normalized = name.lower()
        ensure(normalized not in self.locals, f"Duplicate parameter '{name}'.")
        self.locals.add(normalized)
        self.argnames.append(normalized)

    def _declare_local(self, name: str) -> str:
        normalized = name.lower()
        ensure(normalized not in self.locals, f"Duplicate local '{name}'.")
        self.locals.add(normalized)
        return normalized

    def _require_local(self, name: str) -> str:
        normalized = name.lower()
        ensure(normalized in self.locals, f"Unknown local '{name}'.")
        return normalized

    def _new_temp_local(self, prefix: str) -> str:
        while True:
            name = f"__frb_{prefix}_{self._temp_counter}"
            self._temp_counter += 1
            normalized = name.lower()
            if normalized not in self.locals:
                self.locals.add(normalized)
                return normalized

    def _new_label(self, prefix: str) -> str:
        name = f"{prefix}_{self._label_counter}"
        self._label_counter += 1
        return name

    def _emit(self, opname: str, arg: Any = _NO_ARG) -> None:
        self.instructions.append(InstructionSpec(opname, arg))

    def _emit_label(self, name: str) -> None:
        self.instructions.append(LabelSpec(name))

    def _ensure_final_return(self) -> None:
        last_instr: Optional[InstructionSpec] = None
        for entry in reversed(self.instructions):
            if isinstance(entry, InstructionSpec):
                last_instr = entry
                break
        if not last_instr or last_instr.opname != "RETURN_VALUE":
            self._emit("LOAD_CONST", None)
            self._emit("RETURN_VALUE")

    # ------------------------------------------------------------------
    def _lower_block(self, block: nodes.Block) -> None:
        for stmt in block.statements:
            self._lower_stmt(stmt)

    def _lower_stmt(self, stmt: nodes.Statement) -> None:
        if isinstance(stmt, nodes.Block):
            self._lower_block(stmt)
            return
        if isinstance(stmt, nodes.VarDecl):
            self._lower_var_decl(stmt)
            return
        if isinstance(stmt, nodes.ExprStmt):
            self._lower_expr(stmt.expr)
            self._emit("POP_TOP")
            return
        if isinstance(stmt, nodes.ReturnStmt):
            if stmt.value:
                self._lower_expr(stmt.value)
            else:
                self._emit("LOAD_CONST", None)
            self._emit("RETURN_VALUE")
            return
        if isinstance(stmt, nodes.ForStmt):
            self._lower_for_stmt(stmt)
            return
        raise UnsupportedFeatureError(f"Statement {type(stmt).__name__} is not supported.")

    def _lower_var_decl(self, stmt: nodes.VarDecl) -> None:
        ensure(stmt.names, "Declaration must contain at least one name.")
        if stmt.initializer is not None:
            ensure(len(stmt.names) == 1, "Only single-name declarations can have an initializer.")
            target = self._declare_local(stmt.names[0])
            self._lower_expr(stmt.initializer)
            self._emit("STORE_FAST", target)
            return
        default_value = self._default_value(stmt.type_ref)
        for name in stmt.names:
            target = self._declare_local(name)
            self._emit("LOAD_CONST", default_value)
            self._emit("STORE_FAST", target)

    def _lower_for_stmt(self, stmt: nodes.ForStmt) -> None:
        ensure(isinstance(stmt.target, nodes.Identifier), "For target must be an identifier.")
        ensure(isinstance(stmt.initializer, nodes.AssignmentExpr), "For initializer must be an assignment.")
        init_expr = stmt.initializer
        ensure(isinstance(init_expr.target, nodes.Identifier), "For initializer target must be an identifier.")
        loop_var = self._require_local(stmt.target.name)
        ensure(init_expr.target.name.lower() == loop_var, "For loop initializer must assign the loop variable.")
        direction = stmt.direction or 1
        self._lower_expr(init_expr)
        self._emit("POP_TOP")
        limit_var = self._new_temp_local("limit")
        self._lower_expr(stmt.limit)
        self._emit("STORE_FAST", limit_var)
        step_var = self._new_temp_local("step")
        if stmt.step:
            self._lower_expr(stmt.step)
        else:
            default_step = 1 if direction > 0 else -1
            self._emit("LOAD_CONST", default_step)
        self._emit("STORE_FAST", step_var)
        check_label = self._new_label("for_check")
        end_label = self._new_label("for_end")
        self._emit_label(check_label)
        self._emit("LOAD_FAST", loop_var)
        self._emit("LOAD_FAST", limit_var)
        compare = Compare.LE if direction > 0 else Compare.GE
        self._emit("COMPARE_OP", compare)
        self._emit("POP_JUMP_IF_FALSE", LabelRef(end_label))
        self._lower_block(stmt.body)
        self._emit("LOAD_FAST", loop_var)
        self._emit("LOAD_FAST", step_var)
        opname, arg = self._binary_op_mapping(nodes.BinaryOperator.ADD)
        self._emit(opname, arg)
        self._emit("STORE_FAST", loop_var)
        self._emit("JUMP_BACKWARD", LabelRef(check_label))
        self._emit_label(end_label)

    # ------------------------------------------------------------------
    def _lower_expr(self, expr: nodes.Expression) -> None:
        if isinstance(expr, nodes.Literal):
            self._emit("LOAD_CONST", expr.value)
            return
        if isinstance(expr, nodes.Identifier):
            normalized = expr.name.lower()
            if normalized in self.locals:
                self._emit("LOAD_FAST", normalized)
            else:
                self._load_global(expr.name)
            return
        if isinstance(expr, nodes.MemberAccess):
            self._lower_member_access(expr, push_null=False)
            return
        if isinstance(expr, nodes.BinaryOpExpr):
            self._lower_expr(expr.left)
            self._lower_expr(expr.right)
            opname, arg = self._binary_op_mapping(expr.op)
            self._emit(opname, arg)
            return
        if isinstance(expr, nodes.UnaryOpExpr):
            ensure(expr.op is nodes.UnaryOperator.NEGATE, "Only unary '-' supported.")
            self._lower_expr(expr.operand)
            self._emit("UNARY_NEGATIVE")
            return
        if isinstance(expr, nodes.AssignmentExpr):
            ensure(
                expr.op in {nodes.AssignmentOp.ASSIGN, nodes.AssignmentOp.REF_ASSIGN},
                "Only simple assignments are supported.",
            )
            ensure(isinstance(expr.target, nodes.Identifier), "Assignment target must be an identifier.")
            target = self._require_local(expr.target.name)
            self._lower_expr(expr.value)
            self._emit("STORE_FAST", target)
            self._emit("LOAD_FAST", target)
            return
        if isinstance(expr, nodes.CallExpr):
            self._lower_call(expr)
            return
        if isinstance(expr, nodes.CastExpr):
            self._lower_expr(expr.value)
            return
        raise UnsupportedFeatureError(f"Expression {type(expr).__name__} is not supported.")

    def _load_global(self, name: str, *, push_null: bool = False) -> None:
        self.global_refs.add(name)
        self._emit("LOAD_GLOBAL", _load_global_arg(name, push_null))

    def _binary_op_mapping(self, op: nodes.BinaryOperator) -> tuple[str, Any]:
        mapping = {
            nodes.BinaryOperator.ADD: ("BINARY_OP", CPBinaryOp.ADD.value) if PY311_PLUS else ("BINARY_ADD", _NO_ARG),
            nodes.BinaryOperator.SUB: ("BINARY_OP", CPBinaryOp.SUBTRACT.value) if PY311_PLUS else ("BINARY_SUBTRACT", _NO_ARG),
            nodes.BinaryOperator.MUL: ("BINARY_OP", CPBinaryOp.MULTIPLY.value) if PY311_PLUS else ("BINARY_MULTIPLY", _NO_ARG),
            nodes.BinaryOperator.DIV: ("BINARY_OP", CPBinaryOp.TRUE_DIVIDE.value) if PY311_PLUS else ("BINARY_TRUE_DIVIDE", _NO_ARG),
            nodes.BinaryOperator.INT_DIV: ("BINARY_OP", CPBinaryOp.FLOOR_DIVIDE.value) if PY311_PLUS else ("BINARY_FLOOR_DIVIDE", _NO_ARG),
            nodes.BinaryOperator.MOD: ("BINARY_OP", CPBinaryOp.REMAINDER.value) if PY311_PLUS else ("BINARY_MODULO", _NO_ARG),
            nodes.BinaryOperator.POW: ("BINARY_OP", CPBinaryOp.POWER.value) if PY311_PLUS else ("BINARY_POWER", _NO_ARG),
        }
        ensure(op in mapping, "Unsupported binary operator.")
        opname, arg = mapping[op]
        return (opname, arg) if arg is not _NO_ARG else (opname, _NO_ARG)

    def _default_value(self, type_ref: Optional[nodes.TypeRef]) -> Any:
        if not type_ref:
            return None
        name = type_ref.parts[-1].lower()
        if name in {"int", "integer", "long", "short"}:
            return 0
        if name in {"bool", "boolean"}:
            return False
        if name in {"string", "text"}:
            return ""
        if name == "float":
            return 0.0
        return None

    def _lower_member_access(self, expr: nodes.MemberAccess, *, push_null: bool) -> None:
        self._lower_expr(expr.obj)
        self._emit("LOAD_ATTR", _load_attr_arg(expr.member, push_null))
        if push_null and PY311_PLUS and not PY312_PLUS:
            self._emit("PUSH_NULL")

    def _lower_call(self, expr: nodes.CallExpr) -> None:
        if isinstance(expr.func, nodes.MemberAccess):
            self._lower_member_access(expr.func, push_null=True)
        elif isinstance(expr.func, nodes.Identifier):
            normalized = expr.func.name.lower()
            if normalized in self.locals:
                self._emit("LOAD_FAST", normalized)
                self._maybe_push_null()
            else:
                self._load_global(expr.func.name, push_null=True)
        else:
            raise UnsupportedFeatureError("Call target is not supported.")
        for argument in expr.args:
            self._lower_expr(argument)
        self._emit(CALL_OPCODE, len(expr.args))

    def _maybe_push_null(self) -> None:
        if PY311_PLUS:
            self._emit("PUSH_NULL")


class ModuleRenderer:
    def __init__(self, compiled: CompiledProgram, artifacts: BytecodeArtifacts) -> None:
        self.compiled = compiled
        self.filename = str(artifacts.source)

    def render(self) -> str:
        lines: List[str] = []
        lines.append("from bytecode import Instr, Bytecode, Label, Compare")
        lines.append("from types import FunctionType")
        lines.append("")
        self._render_bytecodes(lines)
        lines.append("# Code objects")
        for routine in self.compiled.routines:
            lines.append(f"{routine.var_name}_code = {routine.var_name}_bc.to_code()")
        lines.append("")
        lines.append("globals_dict = {}")
        external_globals = self._external_globals()
        if external_globals:
            for name in sorted(external_globals):
                lines.append(f"globals_dict[{name!r}] = {name}")
            lines.append("")
        self._render_functions(lines)
        self._render_classes(lines)
        self._render_free_functions(lines)
        lines.append("globals_dict[\"Main\"]().main()")
        return "\n".join(lines) + "\n"

    def _render_bytecodes(self, lines: List[str]) -> None:
        lines.append("# Bytecode objects")
        for routine in self.compiled.routines:
            label_names = self._collect_label_names(routine)
            for label in label_names:
                lines.append(f"{routine.var_name}_{label} = Label()")
            if label_names:
                lines.append("")
            lines.append(f"{routine.var_name}_bc = Bytecode([")
            for entry in routine.instructions:
                if isinstance(entry, LabelSpec):
                    lines.append(f"    {routine.var_name}_{entry.name},")
                else:
                    arg_text = self._render_instr_arg(routine, entry.arg)
                    if arg_text is None:
                        lines.append(f'    Instr("{entry.opname}"),')
                    else:
                        lines.append(f'    Instr("{entry.opname}", {arg_text}),')
            lines.append("])")
            lines.append(f"{routine.var_name}_bc.argcount = {len(routine.argnames)}")
            lines.append(f"{routine.var_name}_bc.argnames = {routine.argnames!r}")
            lines.append(f"{routine.var_name}_bc.name = {routine.method_name!r}")
            lines.append(f"{routine.var_name}_bc.filename = {self.filename!r}")
            lines.append("")

    def _render_functions(self, lines: List[str]) -> None:
        lines.append("# Function objects")
        for routine in self.compiled.routines:
            lines.append(
                f"{routine.var_name}_func = FunctionType({routine.var_name}_code, globals_dict, {routine.method_name!r})"
            )
        lines.append("")

    def _render_classes(self, lines: List[str]) -> None:
        lines.append("# Classes")
        for class_name, methods in self.compiled.class_methods.items():
            entries: List[str] = []
            for routine in methods:
                ref = f"{routine.var_name}_func"
                if routine.shared:
                    ref = f"staticmethod({ref})"
                entries.append(f"{routine.method_name!r}: {ref}")
            body = ", ".join(entries)
            lines.append(f"{class_name} = type({class_name!r}, (object,), {{{body}}})")
            lines.append(f"globals_dict[{class_name!r}] = {class_name}")
            lines.append("")

    def _render_free_functions(self, lines: List[str]) -> None:
        if not self.compiled.free_functions:
            return
        lines.append("# Free functions")
        for routine in self.compiled.free_functions:
            lines.append(f"globals_dict[{routine.method_name!r}] = {routine.var_name}_func")
        lines.append("")

    def _external_globals(self) -> Set[str]:
        class_names = set(self.compiled.class_methods.keys())
        function_names = {routine.method_name for routine in self.compiled.free_functions}
        return {name for name in self.compiled.global_refs if name not in class_names and name not in function_names}

    def _collect_label_names(self, routine: CompiledRoutine) -> List[str]:
        names: List[str] = []
        seen: Set[str] = set()
        for entry in routine.instructions:
            if isinstance(entry, LabelSpec) and entry.name not in seen:
                seen.add(entry.name)
                names.append(entry.name)
        return names

    def _render_instr_arg(self, routine: CompiledRoutine, arg: Any) -> Optional[str]:
        if arg is _NO_ARG:
            return None
        if isinstance(arg, LabelRef):
            return f"{routine.var_name}_{arg.name}"
        if isinstance(arg, Compare):
            return f"Compare.{arg.name}"
        return repr(arg)


class UnsupportedFeatureError(NotImplementedError):
    pass


def ensure(condition: bool, message: str) -> None:
    if not condition:
        raise UnsupportedFeatureError(message)


def _load_global_arg(name: str, push_null: bool) -> Any:
    if PY311_PLUS:
        return (push_null, name)
    return name


def _load_attr_arg(name: str, push_null: bool) -> Any:
    if PY312_PLUS:
        return (push_null, name)
    return name
