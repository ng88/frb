from __future__ import annotations

from dataclasses import dataclass
from enum import Enum, auto
from typing import Any, Optional


class TokenKind(Enum):
    EOF = auto()
    NEWLINE = auto()
    IDENTIFIER = auto()
    INT = auto()
    FLOAT = auto()
    STRING = auto()
    KEYWORD = auto()
    SYMBOL = auto()


@dataclass(frozen=True)
class Token:
    kind: TokenKind
    value: Any
    text: str
    line: int
    column: int

    def as_keyword(self, token_name: str) -> bool:
        return self.kind is TokenKind.KEYWORD and self.value == token_name

    def as_symbol(self, symbol_name: str) -> bool:
        return self.kind is TokenKind.SYMBOL and self.value == symbol_name


def eof_token(line: int, column: int) -> Token:
    return Token(TokenKind.EOF, None, "", line, column)
