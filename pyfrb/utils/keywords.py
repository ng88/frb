from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Mapping, MutableMapping, Optional


class KeywordError(RuntimeError):
    """Raised when the keyword table cannot be parsed or used."""


@dataclass(frozen=True)
class KeywordEntry:
    token: str
    per_language: Mapping[str, str]


class KeywordRegistry:
    """Loads the historical keyword description file and exposes lookup helpers."""

    def __init__(self, keywords_path: Path | str) -> None:
        self._keywords_path = Path(keywords_path)
        self._languages: List[str] = []
        self._keyword_entries: Dict[str, KeywordEntry] = {}
        self._keywords_by_language: Dict[str, Dict[str, KeywordEntry]] = {}
        self._symbol_lexemes: Dict[str, str] = {}
        self._symbols_by_token: Dict[str, List[str]] = {}
        self._current_language: Optional[str] = None
        self._load()

    # ------------------------------------------------------------------ #
    # public API
    # ------------------------------------------------------------------ #
    @property
    def languages(self) -> Iterable[str]:
        return tuple(self._languages)

    @property
    def current_language(self) -> str:
        if not self._current_language:
            raise KeywordError("No active keyword language set.")
        return self._current_language

    def set_language(self, name: str) -> None:
        upper = name.strip().upper()
        matches = [lang for lang in self._languages if lang.upper() == upper]
        if not matches:
            raise KeywordError(f"Unknown language set '{name}'.")
        self._current_language = matches[0]

    def keyword_entry(self, token: str) -> KeywordEntry:
        try:
            return self._keyword_entries[token]
        except KeyError as exc:  # pragma: no cover - defensive
            raise KeywordError(f"Unknown keyword token '{token}'.") from exc

    def keyword_token(self, lexeme: str, language: Optional[str] = None) -> Optional[str]:
        lang = language or self.current_language
        lookup = self._keywords_by_language.get(lang, {})
        entry = lookup.get(lexeme)
        return entry.token if entry else None

    def symbol_token(self, lexeme: str) -> Optional[str]:
        return self._symbol_lexemes.get(lexeme)

    def symbol_variants(self, token: str) -> List[str]:
        return list(self._symbols_by_token.get(token, ()))

    def all_symbol_lexemes(self) -> Iterable[str]:
        return tuple(self._symbol_lexemes.keys())

    # ------------------------------------------------------------------ #
    # parsing
    # ------------------------------------------------------------------ #
    def _load(self) -> None:
        if not self._keywords_path.exists():
            raise KeywordError(f"Keyword file '{self._keywords_path}' not found.")

        content = self._keywords_path.read_text(encoding="latin-1")
        in_symbol_block = False

        def ensure_languages(parts: List[str]) -> None:
            if not parts:
                raise KeywordError("@KW_SET directive missing languages.")
            self._languages = [p.strip() for p in parts if p.strip()]
            if not self._languages:
                raise KeywordError("@KW_SET must specify at least one language.")
            for lang in self._languages:
                self._keywords_by_language.setdefault(lang, {})

        for raw_line in content.splitlines():
            line = raw_line.strip()
            if not line or line.startswith("#"):
                continue

            if line.startswith("@KW_SET"):
                _, *rest = line.split(";")
                ensure_languages(rest)
                continue

            if line == "@BEGIN_SYMBOL":
                in_symbol_block = True
                continue
            if line == "@END_SYMBOL":
                in_symbol_block = False
                continue

            if line.startswith("+"):
                continue  # auxiliary translations not used by lexer/parser

            if line.startswith("@ALIAS;"):
                if not in_symbol_block:
                    raise KeywordError("@ALIAS directive is only valid in the symbol block.")
                _, token_name, alias = line.split(";", 2)
                alias = alias.strip()
                token_name = token_name.strip()
                self._register_symbol(alias, token_name)
                continue

            if in_symbol_block:
                token_name, lexeme = self._split_symbol_line(line)
                self._register_symbol(lexeme, token_name)
                continue

            token, *lang_words = [part.strip() for part in line.split(";")]
            if not token:
                raise KeywordError(f"Invalid keyword entry: '{line}'.")
            if not self._languages:
                raise KeywordError("Keyword entries encountered before @KW_SET directive.")
            if len(lang_words) < len(self._languages):
                raise KeywordError(f"Keyword '{token}' is missing translations.")

            translations = {
                lang: lang_words[idx]
                for idx, lang in enumerate(self._languages)
                if lang_words[idx]
            }
            entry = KeywordEntry(token=token, per_language=translations)
            self._keyword_entries[token] = entry
            for lang, word in translations.items():
                if not word:
                    continue
                lang_map = self._keywords_by_language.setdefault(lang, {})
                lang_map[word] = entry

        if not self._current_language and self._languages:
            self._current_language = self._languages[0]

    # ------------------------------------------------------------------ #
    def _split_symbol_line(self, line: str) -> tuple[str, str]:
        parts = [part.strip() for part in line.split(";", 1)]
        if len(parts) != 2 or not parts[0] or not parts[1]:
            raise KeywordError(f"Invalid symbol definition: '{line}'.")
        return parts[0], parts[1]

    def _register_symbol(self, lexeme: str, token_name: str) -> None:
        if not lexeme:
            raise KeywordError("Symbol lexeme cannot be empty.")
        self._symbol_lexemes[lexeme] = token_name
        bucket = self._symbols_by_token.setdefault(token_name, [])
        if lexeme not in bucket:
            bucket.append(lexeme)
