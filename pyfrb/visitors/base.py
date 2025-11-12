from __future__ import annotations

from typing import Any

from src2.ast import nodes


class NodeVisitor:
    """Generic AST visitor with dynamic dispatch."""

    def visit(self, node: nodes.Node | None) -> Any:
        if node is None:
            return None
        method_name = f"visit_{type(node).__name__}"
        visitor = getattr(self, method_name, self.generic_visit)
        return visitor(node)

    def generic_visit(self, node: nodes.Node) -> Any:  # pragma: no cover - debugging aid
        raise NotImplementedError(f"No visit_{type(node).__name__} method defined.")
