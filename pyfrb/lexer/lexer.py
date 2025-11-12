from __future__ import annotations

from dataclasses import dataclass
from typing import Iterator, List, Optional

from src2.utils.keywords import KeywordRegistry, KeywordError

from .tokens import Token, TokenKind, eof_token


class LexerError(RuntimeError):
    pass


@dataclass
class LexerConfig:
    keywords: KeywordRegistry


class Lexer:
    def __init__(self, source: str, filename: str, config: LexerConfig) -> None:
        self.filename = filename
        self.config = config
        normalized = source.replace("\r\n", "\n").replace("\r", "\n")
        self.source = normalized
        self.length = len(self.source)
        self.index = 0
        self.line = 1
        self.column = 1
        self._deflang_pending = False
        self._symbol_lexemes = sorted(config.keywords.all_symbol_lexemes(), key=len, reverse=True)

    # ------------------------------------------------------------------ #
    def tokenize(self) -> Iterator[Token]:
        while True:
            token = self._next_token()
            if token is None:
                continue
            yield token
            if token.kind is TokenKind.EOF:
                break

    # ------------------------------------------------------------------ #
    def _next_token(self) -> Optional[Token]:
        self._skip_blanks()

        if self.index >= self.length:
            return eof_token(self.line, self.column)

        ch = self._peek()

        if ch == "\n":
            self._advance()
            return Token(TokenKind.NEWLINE, None, "\n", self.line - 1, 1)

        if ch == "'":
            self._consume_comment()
            return None

        if ch == '"':
            return self._string_literal()

        if ch.isdigit():
            return self._number_literal()

        if ch == "&":
            token = self._prefixed_numeric()
            if token:
                return token

        if ch.isalpha() or ch == "_":
            maybe = self._identifier_or_keyword()
            if maybe is not None:
                return maybe
            return None

        symbol_token = self._match_symbol()
        if symbol_token:
            return symbol_token

        raise LexerError(f"Unexpected character '{ch}' at {self.line}:{self.column} in {self.filename}")

    # ------------------------------------------------------------------ #
    def _skip_blanks(self) -> None:
        while True:
            start = self.index
            while self.index < self.length:
                ch = self._peek()
                if ch in (" ", "\t", "\v", "\f"):
                    self._advance()
                    continue
                break

            if self._consume_line_continuation():
                continue

            if self.index == start:
                break

    def _consume_line_continuation(self) -> bool:
        save_index = self.index
        save_line = self.line
        save_column = self.column

        while self.index < self.length and self._peek() in (" ", "\t", "\v", "\f"):
            self._advance()

        if self._peek() == "_" and self._peek(1) == "\n":
            self._advance()  # underscore
            self._advance()  # newline
            return True

        self.index = save_index
        self.line = save_line
        self.column = save_column
        return False

    def _consume_comment(self) -> None:
        while self.index < self.length and self._peek() != "\n":
            self._advance()

    def _string_literal(self) -> Token:
        line, column = self.line, self.column
        self._advance()  # opening quote
        chars: List[str] = []
        escaped = False

        while self.index < self.length:
            ch = self._peek()
            if ch == "\n":
                raise LexerError(f"Unterminated string literal at {line}:{column}")
            self._advance()
            if escaped:
                chars.append(ch)
                escaped = False
                continue
            if ch == "\\":
                escaped = True
                continue
            if ch == '"':
                break
            chars.append(ch)
        else:
            raise LexerError(f"Unterminated string literal at {line}:{column}")

        value = "".join(chars)
        return Token(TokenKind.STRING, value, value, line, column)

    def _number_literal(self) -> Token:
        line, column = self.line, self.column
        start = self.index
        while self.index < self.length and self._peek().isdigit():
            self._advance()

        is_float = False
        if self._peek() == "." and self._peek(1).isdigit():
            is_float = True
            self._advance()
            while self.index < self.length and self._peek().isdigit():
                self._advance()

        if self._peek() in ("e", "E"):
            is_float = True
            self._advance()
            if self._peek() in ("+", "-"):
                self._advance()
            if not self._peek().isdigit():
                raise LexerError(f"Malformed exponent at {self.line}:{self.column}")
            while self.index < self.length and self._peek().isdigit():
                self._advance()

        text = self.source[start:self.index]
        if is_float:
            return Token(TokenKind.FLOAT, float(text), text, line, column)
        return Token(TokenKind.INT, int(text, 10), text, line, column)

    def _prefixed_numeric(self) -> Optional[Token]:
        if self._peek() != "&":
            return None
        line, column = self.line, self.column
        prefix = self._peek(1).upper()
        handler = {
            "H": 16,
            "B": 2,
            "O": 8,
        }.get(prefix)

        if handler:
            self._advance()  # &
            self._advance()  # prefix letter
            start = self.index
            valid = "0123456789ABCDEF"[: handler] if handler == 16 else (
                "01" if handler == 2 else "01234567"
            )
            while self.index < self.length and self._peek().upper() in valid:
                self._advance()
            digits = self.source[start:self.index]
            if not digits:
                raise LexerError(f"Expected digits for base prefix '&{prefix}'.")
            return Token(TokenKind.INT, int(digits, handler), digits, line, column)

        if prefix in ("L", "S"):
            self._advance()
            self._advance()
            start = self.index
            while self.index < self.length and self._peek().isdigit():
                self._advance()
            digits = self.source[start:self.index]
            if not digits:
                raise LexerError(f"Expected decimal digits after '&{prefix}'.")
            return Token(TokenKind.INT, int(digits, 10), digits, line, column)

        if prefix == "F":
            self._advance()
            self._advance()
            token = self._number_literal()
            return Token(TokenKind.FLOAT, token.value, token.text, line, column)

        return None

    def _identifier_or_keyword(self) -> Optional[Token]:
        line, column = self.line, self.column
        start = self.index
        while self.index < self.length:
            ch = self._peek()
            if ch.isalnum() or ch == "_":
                self._advance()
                continue
            break
        text = self.source[start:self.index]
        if self._deflang_pending:
            self._deflang_pending = False
            try:
                self.config.keywords.set_language(text)
            except KeywordError as exc:
                raise LexerError(str(exc)) from exc
            return None
        if text.lower() == "deflang":
            self._deflang_pending = True
            return None
        token_name = self.config.keywords.keyword_token(text)
        if token_name:
            return Token(TokenKind.KEYWORD, token_name, text, line, column)
        return Token(TokenKind.IDENTIFIER, text, text, line, column)

    def _match_symbol(self) -> Optional[Token]:
        for lexeme in self._symbol_lexemes:
            if self.source.startswith(lexeme, self.index):
                token_name = self.config.keywords.symbol_token(lexeme)
                if not token_name:
                    continue
                line, column = self.line, self.column
                for _ in range(len(lexeme)):
                    self._advance()
                if token_name == "OP_NEWLINE":
                    return Token(TokenKind.NEWLINE, None, lexeme, line, column)
                return Token(TokenKind.SYMBOL, token_name, lexeme, line, column)
        return None

    # ------------------------------------------------------------------ #
    def _peek(self, lookahead: int = 0) -> str:
        idx = self.index + lookahead
        if idx >= self.length:
            return "\0"
        return self.source[idx]

    def _advance(self) -> None:
        if self.index >= self.length:
            return
        ch = self.source[self.index]
        self.index += 1
        if ch == "\n":
            self.line += 1
            self.column = 1
        else:
            self.column += 1
