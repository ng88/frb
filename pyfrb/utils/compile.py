import importlib.util
import sys
import types
import marshal
import py_compile
import tempfile
import textwrap
from pathlib import Path


def load_generated_module(path: Path):
    """Import the generated FrB->bytecode module from a file path."""
    spec = importlib.util.spec_from_file_location("frb_generated", path)
    module = importlib.util.module_from_spec(spec)
    assert spec.loader is not None
    spec.loader.exec_module(module)
    return module


def build_frozen_source(mod) -> str:
    """
    Build a minimal Python module source that:
      - finds all *_code objects in the generated module,
      - assumes names are of the form ClassName_MethodName_code
        (e.g. Debug_println_code, Main_main_code, Main_fortest_code),
      - embeds their marshalled bytes,
      - rebuilds functions and classes,
      - calls Main().main() if available.

    Labels / Bytecode / Compare, etc. are ignored here: only CodeType objects matter.
    """

    # 1) Collect all CodeType globals ending with "_code"
    codes_by_class: dict[str, dict[str, types.CodeType]] = {}
    # For free functions with no class prefix (if any in future)
    free_functions: dict[str, types.CodeType] = {}

    for name, obj in vars(mod).items():
        if isinstance(obj, types.CodeType) and name.endswith("_code"):
            base = name[:-5]  # strip "_code"
            parts = base.split("_")
            if len(parts) >= 2:
                cls_name = parts[0]
                method_name = "_".join(parts[1:])
                codes_by_class.setdefault(cls_name, {})[method_name] = obj
            else:
                # No class prefix; treat as free function
                func_name = base
                free_functions[func_name] = obj

    if not codes_by_class and not free_functions:
        raise RuntimeError("No *_code CodeType objects found in generated module")

    # 2) Marshal them to bytes
    # keys like "Main_main", "Main_fortest", "Debug_println", or "func"
    code_bytes: dict[str, bytes] = {}
    for cls_name, methods in codes_by_class.items():
        for method_name, code_obj in methods.items():
            key = f"{cls_name}_{method_name}"
            code_bytes[key] = marshal.dumps(code_obj)

    for func_name, code_obj in free_functions.items():
        key = func_name
        code_bytes[key] = marshal.dumps(code_obj)

    # Class layout info: { "Main": ["Add", "main", "fortest"], "Debug": ["println"], ... }
    class_layout: dict[str, list[str]] = {
        cls_name: sorted(methods.keys()) for cls_name, methods in codes_by_class.items()
    }

    free_func_names = sorted(free_functions.keys())

    # 3) Generate Python source for the frozen module
    code_bytes_lines = []
    for key, data in code_bytes.items():
        code_bytes_lines.append(f"    {key!r}: {repr(data)},")
    code_bytes_src = "\n".join(code_bytes_lines)

    class_layout_lines = []
    for cls_name, methods in class_layout.items():
        methods_list = ", ".join(repr(m) for m in methods)
        class_layout_lines.append(f"    {cls_name!r}: [{methods_list}],")
    class_layout_src = "\n".join(class_layout_lines) or "    # no classes\n"

    free_funcs_list_src = ", ".join(repr(n) for n in free_func_names)

    source = f"""
    import marshal
    import types
    import builtins

    # Marshalled code objects
    CODE_BYTES = {{
{code_bytes_src}
    }}

    # Mapping: class name -> list of method names
    CLASS_LAYOUT = {{
{class_layout_src}
    }}

    # List of free (non-class) function names
    FREE_FUNCTIONS = [{free_funcs_list_src}]

    # Global environment for the functions
    globals_dict = {{}}

    # Populate some common builtins that FrB code may use
    for name in ["print", "input", "int", "str", "len", "range"]:
        if hasattr(builtins, name):
            globals_dict[name] = getattr(builtins, name)

    functions = {{}}

    # 1) Recreate code objects and wrap them into functions
    for key, data in CODE_BYTES.items():
        code_obj = marshal.loads(data)
        # func_name = key itself (e.g. "Main_Add", "Debug_println")
        func_name = key
        functions[func_name] = types.FunctionType(code_obj, globals_dict, func_name)

    # 2) Build classes from CLASS_LAYOUT
    for cls_name, methods in CLASS_LAYOUT.items():
        namespace = {{}}
        for m in methods:
            key = f"{{cls_name}}_{{m}}"
            func = functions[key]
            # attach as staticmethod on the class
            namespace[m] = builtins.staticmethod(func)
            # also expose as a global function under the method name
            globals_dict[m] = func
            globals()[m] = func

        cls = type(cls_name, (object,), namespace)
        globals_dict[cls_name] = cls
        globals()[cls_name] = cls

    # 3) Free functions (if any)
    for fname in FREE_FUNCTIONS:
        func = functions[fname]
        globals_dict[fname] = func
        globals()[fname] = func

    # 4) Expose globals_dict into module globals
    globals().update(globals_dict)

    # 5) Run Main().main() if available
    Main = globals_dict.get("Main")
    if Main is not None and hasattr(Main, "main"):
        Main().main()
    """

    return textwrap.dedent(source)


def freeze_to_pyc(input_file: str, output_pyc: str | None = None):
    input_path = Path(input_file).resolve()
    if output_pyc is None:
        output_pyc = str(input_path.with_suffix(".frozen.pyc"))
    output_pyc_path = Path(output_pyc)

    # 1) Import the generated module (the one that uses Bytecode/Label/etc. and .to_code())
    mod = load_generated_module(input_path)

    # 2) Build the "frozen" Python source that no longer uses bytecode.Bytecode, Label, etc.
    frozen_src = build_frozen_source(mod)

    # 3) Write this source to a temporary .py file and compile to .pyc
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp_py = Path(tmpdir) / (input_path.stem + "_frozen.py")
        tmp_py.write_text(frozen_src, encoding="utf-8")

        py_compile.compile(str(tmp_py), cfile=str(output_pyc_path), doraise=True)

    print(f"Frozen pyc written to: {output_pyc_path}")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} path/to/generated_file.py [output.pyc]")
        raise SystemExit(1)

    input_file = sys.argv[1]
    output_pyc = sys.argv[2] if len(sys.argv) >= 3 else None
    freeze_to_pyc(input_file, output_pyc)
