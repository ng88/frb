from __future__ import annotations

import base64
import math
from dataclasses import dataclass
from typing import Any, Dict, Iterable, List, Optional

from src2.ast import nodes


class ReturnSignal(Exception):
    def __init__(self, value: Any = None) -> None:
        super().__init__("return")
        self.value = value


def execute_program(program_ast: nodes.Program) -> None:
    executor = ProgramExecutor(program_ast)
    executor.run()


class ProgramExecutor:
    def __init__(self, program_ast: nodes.Program) -> None:
        self.program_ast = program_ast
        self.classes: Dict[str, RuntimeClass] = {}
        self.builtins: Dict[str, Any] = {
            "debug": DebugRuntime(self),
        }
        self._register_classes(program_ast.declarations, namespace=None)

    # ------------------------------------------------------------------ #
    def _register_classes(self, declarations: Iterable[nodes.ClassDecl], namespace: Optional[str]) -> None:
        for decl in declarations:
            full_name = decl.name if not namespace else f"{namespace}.{decl.name}"
            runtime_class = RuntimeClass(self, decl, full_name, namespace)
            self.classes[full_name] = runtime_class
            if namespace is None:
                # allow top-level lookup by simple name
                self.classes.setdefault(decl.name, runtime_class)
            runtime_class.register_nested()

    # ------------------------------------------------------------------ #
    def run(self) -> None:
        main_class = self.classes.get("Main")
        if not main_class:
            raise RuntimeError("Entry class 'Main' not found.")
        main_method = main_class.resolve_method("main", 0, require_shared=True)
        if not main_method:
            raise RuntimeError("Entry method 'Main.main()' not found.")
        main_class.invoke(None, main_method, [])

    # ------------------------------------------------------------------ #
    def resolve_class(self, type_ref: nodes.TypeRef) -> RuntimeClass:
        name = ".".join(type_ref.parts)
        runtime_class = self.classes.get(name)
        if not runtime_class and len(type_ref.parts) == 1:
            runtime_class = self.classes.get(type_ref.parts[0])
        if not runtime_class:
            raise RuntimeError(f"Unknown class '{name}'.")
        return runtime_class


class RuntimeClass:
    def __init__(self, executor: ProgramExecutor, decl: nodes.ClassDecl, full_name: str, namespace: Optional[str]) -> None:
        self.executor = executor
        self.decl = decl
        self.full_name = full_name
        self.namespace = namespace
        self.name = decl.name
        self.parent_name = namespace
        self.shared_initialized = False
        self.shared_values: Dict[str, Any] = {}
        self.events_shared: Dict[str, EventSlot] = {}
        self.instance_field_inits: List[nodes.FieldDecl] = []
        self.shared_field_inits: List[nodes.FieldDecl] = []
        self.methods: Dict[str, List[nodes.MethodDecl]] = {}
        self.events_instance: List[nodes.EventDecl] = []
        self.nested: Dict[str, RuntimeClass] = {}

        for member in decl.members:
            if isinstance(member, nodes.FieldDecl):
                if member.shared:
                    self.shared_field_inits.append(member)
                else:
                    self.instance_field_inits.append(member)
            elif isinstance(member, nodes.MethodDecl):
                self.methods.setdefault(member.name.lower(), []).append(member)
            elif isinstance(member, nodes.EventDecl):
                if member.shared:
                    self.events_shared[member.name.lower()] = EventSlot()
                else:
                    self.events_instance.append(member)

    # ------------------------------------------------------------------ #
    def register_nested(self) -> None:
        for member in self.decl.members:
            if isinstance(member, nodes.ClassDecl):
                full_name = f"{self.full_name}.{member.name}"
                nested = RuntimeClass(self.executor, member, full_name, self.full_name)
                self.executor.classes[full_name] = nested
                nested.register_nested()
                self.nested[member.name] = nested
                self.nested[member.name.lower()] = nested

    # ------------------------------------------------------------------ #
    def ensure_initialized(self) -> None:
        if self.shared_initialized:
            return
        for field in self.shared_field_inits:
            value = ExpressionEvaluator(self.executor, None, self).evaluate(field.initializer) if field.initializer else self._default_value(field.type_ref)
            self.shared_values[field.name.lower()] = value
            if isinstance(value, EventSlot):
                self.events_shared[field.name.lower()] = value
        self.shared_initialized = True

    # ------------------------------------------------------------------ #
    def instantiate(self) -> RuntimeInstance:
        self.ensure_initialized()
        instance = RuntimeInstance(self)
        evaluator = ExpressionEvaluator(self.executor, instance, self)
        for field in self.instance_field_inits:
            value = evaluator.evaluate(field.initializer) if field.initializer else self._default_value(field.type_ref)
            instance.fields[field.name.lower()] = value
        for event in self.events_instance:
            instance.events[event.name.lower()] = EventSlot()
        self._run_constructor(instance, [])
        return instance

    def instantiate_with_args(self, args: List[Any]) -> RuntimeInstance:
        self.ensure_initialized()
        instance = RuntimeInstance(self)
        evaluator = ExpressionEvaluator(self.executor, instance, self)
        for field in self.instance_field_inits:
            value = evaluator.evaluate(field.initializer) if field.initializer else self._default_value(field.type_ref)
            instance.fields[field.name.lower()] = value
        for event in self.events_instance:
            instance.events[event.name.lower()] = EventSlot()
        self._run_constructor(instance, args)
        return instance

    def _run_constructor(self, instance: RuntimeInstance, args: List[Any]) -> None:
        ctor = self.resolve_method("initialize", len(args), require_shared=False)
        if ctor:
            self.invoke(instance, ctor, args)

    # ------------------------------------------------------------------ #
    def resolve_method(self, name: str, argc: int, require_shared: Optional[bool]) -> Optional[nodes.MethodDecl]:
        candidates = self.methods.get(name.lower())
        if not candidates and self.parent_name:
            parent = self.executor.classes.get(self.parent_name)
            if parent:
                return parent.resolve_method(name, argc, require_shared)
        if not candidates:
            return None
        for method in candidates:
            if require_shared is not None and method.shared != require_shared:
                continue
            if len(method.parameters) == argc:
                return method
        return None

    # ------------------------------------------------------------------ #
    def invoke(self, instance: Optional[RuntimeInstance], method: nodes.MethodDecl, args: List[Any]) -> Any:
        self.ensure_initialized()
        frame = Frame(instance, self)
        for param, value in zip(method.parameters, args):
            frame.locals[param.name.lower()] = value
        executor = StatementExecutor(self.executor, frame)
        try:
            executor.execute_block(method.body)
        except ReturnSignal as signal:
            return signal.value
        return None

    # ------------------------------------------------------------------ #
    def _default_value(self, type_ref: Optional[nodes.TypeRef]) -> Any:
        if not type_ref:
            return None
        name = type_ref.parts[-1].lower()
        if name in ("int", "integer", "long", "short"):
            return 0
        if name in ("bool", "boolean"):
            return False
        if name in ("string", "text"):
            return ""
        if name == "float":
            return 0.0
        return None


class RuntimeInstance:
    def __init__(self, runtime_class: RuntimeClass) -> None:
        self.runtime_class = runtime_class
        self.fields: Dict[str, Any] = {}
        self.events: Dict[str, EventSlot] = {}

    def get_field(self, name: str) -> Any:
        if name.lower() in self.fields:
            return self.fields[name.lower()]
        raise RuntimeError(f"Unknown field '{name}'.")

    def set_field(self, name: str, value: Any) -> None:
        self.fields[name.lower()] = value


@dataclass
class Frame:
    instance: Optional[RuntimeInstance]
    runtime_class: RuntimeClass
    locals: Dict[str, Any] = None

    def __post_init__(self) -> None:
        if self.locals is None:
            self.locals = {}


class StatementExecutor:
    def __init__(self, executor: ProgramExecutor, frame: Frame) -> None:
        self.executor = executor
        self.frame = frame
        self.expr = ExpressionEvaluator(executor, frame.instance, frame.runtime_class, frame)

    def execute_block(self, block: nodes.Block) -> None:
        for stmt in block.statements:
            self.execute(stmt)

    def execute(self, stmt: nodes.Statement) -> None:
        if isinstance(stmt, nodes.Block):
            self.execute_block(stmt)
        elif isinstance(stmt, nodes.VarDecl):
            for name in stmt.names:
                value = self.expr.evaluate(stmt.initializer) if stmt.initializer else self.expr.default_value(stmt.type_ref)
                self.frame.locals[name.lower()] = value
        elif isinstance(stmt, nodes.ExprStmt):
            self.expr.evaluate(stmt.expr)
        elif isinstance(stmt, nodes.ReturnStmt):
            value = self.expr.evaluate(stmt.value) if stmt.value else None
            raise ReturnSignal(value)
        elif isinstance(stmt, nodes.IfStmt):
            executed = False
            for cond, block in stmt.branches:
                if self.expr.boolean_value(self.expr.evaluate(cond)):
                    self.execute_block(block)
                    executed = True
                    break
            if not executed and stmt.else_block:
                self.execute_block(stmt.else_block)
        elif isinstance(stmt, nodes.ForStmt):
            self._execute_for(stmt)
        else:
            raise RuntimeError(f"Unsupported statement '{type(stmt).__name__}'.")

    def _execute_for(self, stmt: nodes.ForStmt) -> None:
        init_value = self.expr.evaluate(stmt.initializer)
        target = stmt.initializer.target
        direction = stmt.direction or 1
        limit = self.expr.evaluate(stmt.limit)
        step = self.expr.evaluate(stmt.step) if stmt.step else (1 if direction > 0 else -1)

        def assign(value: Any) -> None:
            self.expr.assign(target, value)

        value = init_value
        assign(value)

        comparator = (lambda v: v <= limit) if direction > 0 else (lambda v: v >= limit)

        while comparator(value):
            self.execute_block(stmt.body)
            value = value + step
            assign(value)


class ExpressionEvaluator:
    def __init__(self, executor: ProgramExecutor, instance: Optional[RuntimeInstance], runtime_class: RuntimeClass, frame: Optional[Frame] = None) -> None:
        self.executor = executor
        self.instance = instance
        self.runtime_class = runtime_class
        self.frame = frame

    def evaluate(self, expr: Optional[nodes.Expression]) -> Any:
        if expr is None:
            return None
        if isinstance(expr, nodes.Literal):
            return expr.value
        if isinstance(expr, nodes.Identifier):
            return self._resolve_identifier(expr.name)
        if isinstance(expr, nodes.MemberAccess):
            target = self.evaluate(expr.obj)
            return self._resolve_member(target, expr.member)
        if isinstance(expr, nodes.CallExpr):
            return self._call(expr)
        if isinstance(expr, nodes.BinaryOpExpr):
            return self._binary(expr)
        if isinstance(expr, nodes.UnaryOpExpr):
            return self._unary(expr)
        if isinstance(expr, nodes.AssignmentExpr):
            value = self.evaluate(expr.value)
            self.assign(expr.target, value)
            return value
        if isinstance(expr, nodes.NewExpr):
            runtime_class = self.executor.resolve_class(expr.type_ref)
            args = [self.evaluate(arg) for arg in expr.args]
            return runtime_class.instantiate_with_args(args)
        if isinstance(expr, nodes.CastExpr):
            return self.evaluate(expr.value)
        return None

    def default_value(self, type_ref: Optional[nodes.TypeRef]) -> Any:
        if not type_ref:
            return None
        name = type_ref.parts[-1].lower()
        if name in ("int", "integer", "long", "short"):
            return 0
        if name in ("bool", "boolean"):
            return False
        if name in ("string", "text"):
            return ""
        if name == "float":
            return 0.0
        runtime_class = self.executor.resolve_class(type_ref)
        return runtime_class.instantiate()

    def assign(self, target: nodes.Expression, value: Any) -> None:
        if isinstance(target, nodes.Identifier):
            name = target.name.lower()
            if self.frame and name in self.frame.locals:
                self.frame.locals[name] = value
                return
            if self.instance and name in self.instance.fields:
                self.instance.fields[name] = value
                return
            if name in self.runtime_class.shared_values:
                self.runtime_class.shared_values[name] = value
                return
            if self.frame:
                self.frame.locals[name] = value
                return
        if isinstance(target, nodes.MemberAccess):
            receiver = self.evaluate(target.obj)
            if isinstance(receiver, RuntimeInstance):
                receiver.fields[target.member.lower()] = value
                return
            if isinstance(receiver, RuntimeClass):
                receiver.shared_values[target.member.lower()] = value
                return
        raise RuntimeError("Unsupported assignment target.")

    def boolean_value(self, value: Any) -> bool:
        return bool(value)

    # ------------------------------------------------------------------ #
    def _resolve_identifier(self, name: str) -> Any:
        key = name.lower()
        if self.frame and key in self.frame.locals:
            return self.frame.locals[key]
        if self.instance and key in self.instance.fields:
            return self.instance.fields[key]
        if key == "me" and self.instance:
            return self.instance
        if key == self.runtime_class.name.lower():
            return self.runtime_class
        if self.runtime_class.parent_name:
            parent = self.executor.classes.get(self.runtime_class.parent_name)
            if parent:
                nested = parent.nested.get(name) or parent.nested.get(name.lower())
                if nested:
                    return nested
        if key in self.runtime_class.shared_values:
            return self.runtime_class.shared_values[key]
        if key in self.executor.builtins:
            return self.executor.builtins[key]
        methods = self.runtime_class.methods.get(key)
        if methods:
            if self.instance and any(not m.shared for m in methods):
                return MethodReference(self.runtime_class, self.instance, name)
            if any(m.shared for m in methods):
                return MethodReference(self.runtime_class, None, name)
        runtime_class = self.executor.classes.get(name) or self.executor.classes.get(name.capitalize())
        if runtime_class:
            runtime_class.ensure_initialized()
            return runtime_class
        return None

    def _resolve_member(self, target: Any, member: str) -> Any:
        key = member.lower()
        if isinstance(target, RuntimeInstance):
            if key in target.fields:
                return target.fields[key]
            if key in target.events:
                return target.events[key]
            methods = target.runtime_class.methods.get(key)
            if methods:
                if any(not m.shared for m in methods):
                    return MethodReference(target.runtime_class, target, member)
                if any(m.shared for m in methods):
                    return MethodReference(target.runtime_class, None, member)
        if isinstance(target, RuntimeClass):
            target.ensure_initialized()
            if key in target.shared_values:
                return target.shared_values[key]
            if key in target.events_shared:
                return target.events_shared[key]
            nested = target.nested.get(member)
            if nested:
                return nested
            methods = target.methods.get(key)
            if methods and any(m.shared for m in methods):
                return MethodReference(target, None, member)
        if hasattr(target, member):
            return getattr(target, member)
        raise RuntimeError(f"Member '{member}' not found.")

    def _call(self, call: nodes.CallExpr) -> Any:
        func = self.evaluate(call.func)
        args = [self.evaluate(arg) for arg in call.args]
        if isinstance(func, (RuntimeInstance, RuntimeClass)):
            overloaded = self._apply_operator(func, "()", args)
            if overloaded is not None:
                return overloaded
        if isinstance(func, MethodReference):
            return func.invoke(args)
        if isinstance(func, RuntimeClass):
            return func.instantiate_with_args(args)
        if isinstance(func, EventSlot):
            return func.fire(args)
        if callable(func):
            return func(*args)
        raise RuntimeError("Unsupported callable.")

    def _binary(self, expr: nodes.BinaryOpExpr) -> Any:
        left = self.evaluate(expr.left)
        op = expr.op
        if op in (nodes.BinaryOperator.LOG_AND, nodes.BinaryOperator.LOG_OR):
            if op is nodes.BinaryOperator.LOG_AND:
                return self.boolean_value(left) and self.boolean_value(self.evaluate(expr.right))
            return self.boolean_value(left) or self.boolean_value(self.evaluate(expr.right))
        right = self.evaluate(expr.right)
        symbol = self._binary_symbol(op)
        if symbol:
            overloaded = self._apply_operator(left, symbol, [right])
            if overloaded is not None:
                return overloaded
        if isinstance(left, MethodReference):
            left = left.invoke([])
        if isinstance(right, MethodReference):
            right = right.invoke([])
        if op is nodes.BinaryOperator.ADD:
            return left + right
        if op is nodes.BinaryOperator.SUB:
            return left - right
        if op is nodes.BinaryOperator.MUL:
            return left * right
        if op is nodes.BinaryOperator.DIV:
            return left / right
        if op is nodes.BinaryOperator.INT_DIV:
            return left // right
        if op is nodes.BinaryOperator.POW:
            return left ** right
        if op is nodes.BinaryOperator.MOD:
            return left % right
        if op is nodes.BinaryOperator.CONCAT:
            return f"{left}{right}"
        if op is nodes.BinaryOperator.EQ:
            return left == right
        if op is nodes.BinaryOperator.NE:
            return left != right
        if op is nodes.BinaryOperator.LT:
            return left < right
        if op is nodes.BinaryOperator.GT:
            return left > right
        if op is nodes.BinaryOperator.LE:
            return left <= right
        if op is nodes.BinaryOperator.GE:
            return left >= right
        if op is nodes.BinaryOperator.IS:
            return left is right
        if op is nodes.BinaryOperator.IN:
            return left in right
        raise RuntimeError(f"Unsupported operator {op}.")

    def _unary(self, expr: nodes.UnaryOpExpr) -> Any:
        value = self.evaluate(expr.operand)
        if expr.op is nodes.UnaryOperator.NEGATE:
            overloaded = self._apply_operator(value, "-", [])
            if overloaded is not None:
                return overloaded
            return -value
        if expr.op is nodes.UnaryOperator.NOT:
            return not value
        if expr.op in (nodes.UnaryOperator.POST_INC, nodes.UnaryOperator.POST_DEC):
            new_value = value + (1 if expr.op is nodes.UnaryOperator.POST_INC else -1)
            self.assign(expr.operand, new_value)
            return new_value
        return value

    def _apply_operator(self, target: Any, symbol: str, args: List[Any]) -> Any:
        if isinstance(target, RuntimeInstance):
            method = target.runtime_class.resolve_method(f"operator{symbol}", len(args), require_shared=False)
            if method:
                return target.runtime_class.invoke(target, method, args)
        if isinstance(target, RuntimeClass):
            method = target.resolve_method(f"operator{symbol}", len(args), require_shared=True)
            if method:
                return target.invoke(None, method, args)
        return None

    def _binary_symbol(self, op: nodes.BinaryOperator) -> Optional[str]:
        mapping = {
            nodes.BinaryOperator.ADD: "+",
            nodes.BinaryOperator.SUB: "-",
            nodes.BinaryOperator.MUL: "*",
            nodes.BinaryOperator.DIV: "/",
            nodes.BinaryOperator.INT_DIV: "\\",
            nodes.BinaryOperator.POW: "^",
            nodes.BinaryOperator.MOD: "Mod",
            nodes.BinaryOperator.CONCAT: "&",
        }
        return mapping.get(op)


class MethodReference:
    def __init__(self, runtime_class: RuntimeClass, instance: Optional[RuntimeInstance], method_name: str) -> None:
        self.runtime_class = runtime_class
        self.instance = instance
        self.method_name = method_name

    def invoke(self, args: List[Any]) -> Any:
        method = self.runtime_class.resolve_method(self.method_name, len(args), require_shared=(self.instance is None))
        if not method:
            raise RuntimeError(f"Method '{self.method_name}' with {len(args)} arguments not found.")
        return self.runtime_class.invoke(self.instance, method, args)


class EventSlot:
    def __init__(self) -> None:
        self.handlers: List[MethodReference] = []

    def add(self, handler: MethodReference) -> None:
        self.handlers.append(handler)

    def remove(self, handler: MethodReference) -> None:
        if handler in self.handlers:
            self.handlers.remove(handler)

    def fire(self, args: List[Any]) -> None:
        for handler in list(self.handlers):
            handler.invoke(args)


class DebugRuntime:
    def __init__(self, executor: ProgramExecutor) -> None:
        self.executor = executor

    def println(self, value: Any) -> None:
        print("" if value is None else value)

    def typename(self, value: Any) -> str:
        if isinstance(value, RuntimeInstance):
            return value.runtime_class.full_name
        if isinstance(value, RuntimeClass):
            return value.full_name
        return type(value).__name__

    def RegisterEvent(self, source: RuntimeInstance, event_slot: EventSlot, target: Optional[RuntimeInstance], handler: MethodReference) -> None:
        if not event_slot:
            raise RuntimeError("Invalid event slot.")
        if not isinstance(handler, MethodReference):
            raise RuntimeError("Event handler must be a method reference.")
        event_slot.add(handler)

    def UnregisterEvent(self, source: RuntimeInstance, event_slot: EventSlot, target: Optional[RuntimeInstance], handler: MethodReference) -> None:
        if event_slot and isinstance(handler, MethodReference):
            event_slot.remove(handler)
