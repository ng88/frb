# PyFrB

PyFrB is the in-progress Python reimplementation of the FrB compiler and runtime. The modules in this directory include the lexer, parser, AST nodes, optimizer passes, bytecode generator, and runtime executor. The current focus is on lowering FrB programs directly into CPython bytecode via the [`bytecode`](https://github.com/vstinner/bytecode) library, with a lightweight linker that builds real Python `FunctionType` objects and classes.

This tree is the place to iterate on new language features, experiment with optimizations, and validate the Python bytecode backend before porting changes back to the legacy interpreter.

## Running a Regression Test

You can run the Python compiler on any of the historical regression inputs that now live under `interpreter/tests`. For example, to compile `00.frb` and execute the generated Python bytecode:

```bash
python3 -m pyfrb.driver.cli interpreter/tests/noregression/00.frb -o build/00.py
python3 build/00.py
```

The CLI accepts any `.frb` program, parses it with the new frontend, lowers the AST to CPython bytecode, emits a runnable Python module, and finally you can execute that module with the system interpreter to validate behaviour.
