from __future__ import annotations

import argparse
from pathlib import Path
from typing import Optional

from src2.parser.parser import parse_source
from src2.utils.keywords import KeywordRegistry
from src2.visitors.optimizer import ConstantFolder
from src2.visitors.bytecodegen import BytecodeArtifacts, PythonBytecodeGenerator


def compile_file(
    source_path: Path,
    output_path: Path,
    language: str,
) -> None:
    keyword_file = Path(__file__).resolve().parents[2] / "src" / "trad" / "frbkeywords.key"
    keywords = KeywordRegistry(keyword_file)
    keywords.set_language(language)
    text = source_path.read_text(encoding="latin-1")
    program = parse_source(text, str(source_path), keywords)
    ConstantFolder().fold(program)

    generator = PythonBytecodeGenerator()
    generator.emit(
        program,
        BytecodeArtifacts(
            source=source_path,
            output=output_path,
            language=language,
        ),
    )


def build_cli(argv: Optional[list[str]] = None) -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Compile .frb sources to python bytecode modules.")
    parser.add_argument("source", type=Path, help="Input .frb file.")
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        help="Output python file (default: same name with .py).",
    )
    parser.add_argument(
        "--lang",
        choices=("EN", "FR"),
        default="EN",
        help="Keyword set to use (default: EN).",
    )
    return parser


def main(argv: Optional[list[str]] = None) -> None:
    parser = build_cli(argv)
    args = parser.parse_args(argv)
    output = args.output or args.source.with_suffix(".py")
    compile_file(args.source, output, args.lang.upper())
    print(f"Wrote {output}")


if __name__ == "__main__":
    main()
