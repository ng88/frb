# FrB Interpreter

This directory contains the legacy C++ FrB interpreter and toolchain. It holds the original lexer, parser, runtime, and regression test suite that can execute FrB sources end-to-end. Use this tree when you need to build or run the reference implementation exactly as it existed in the historical codebase.

Key highlights:
- `trad/` and related folders provide the language keywords and localization tables.
- Interpreter binaries, runtime support, and regression tests all live here.
- Useful for verifying behaviour or comparing the new Python reimplementation against the established reference.

## Build Requirements

You need a C++ toolchain plus the classic parser generators that the original build scripts expect:

- GNU Make (or a compatible `make`)
- A C++17-capable compiler (GCC / Clang)
- `bison++` for the grammar
- `flex++` (or `flex` with C++ mode) for the lexer
- Standard POSIX build utilities (`bash`, `sed`, `awk`, etc.)

On Debian/Ubuntu-based systems you can install the prerequisites with:

```bash
sudo apt-get install g++ make bison++ flex++ pkg-config
```

## Building

The legacy tree still uses the original `Makefile`s. From the repository root, run:

```bash
cd interpreter
make
```

This will regenerate the lexer/parser sources (via `flex++` and `bison++`) and produce the FrB interpreter binaries inside the `interpreter/` hierarchy. Individual components (such as the parser) can also be built directly (e.g., `make -C parser`) if you only need to test one subsystem.
