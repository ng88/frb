from __future__ import annotations

from dataclasses import dataclass, field
from enum import Enum, auto
from typing import List, Optional, Sequence, Tuple


class Scope(Enum):
    DEFAULT = auto()
    PUBLIC = auto()
    PRIVATE = auto()
    PROTECTED = auto()
    FRIEND = auto()


class ClassKind(Enum):
    CLASS = auto()
    INTERFACE = auto()
    MODULE = auto()


class AssignmentOp(Enum):
    ASSIGN = auto()
    REF_ASSIGN = auto()
    ADD_ASSIGN = auto()
    SUB_ASSIGN = auto()
    MUL_ASSIGN = auto()
    DIV_ASSIGN = auto()
    INT_DIV_ASSIGN = auto()
    POW_ASSIGN = auto()


class UnaryOperator(Enum):
    NEGATE = auto()
    NOT = auto()
    CARDINAL = auto()
    PRE_INC = auto()
    PRE_DEC = auto()
    POST_INC = auto()
    POST_DEC = auto()


class BinaryOperator(Enum):
    ADD = auto()
    SUB = auto()
    MUL = auto()
    DIV = auto()
    INT_DIV = auto()
    POW = auto()
    MOD = auto()
    CONCAT = auto()
    EQ = auto()
    NE = auto()
    LT = auto()
    GT = auto()
    LE = auto()
    GE = auto()
    IS = auto()
    IN = auto()
    LOG_AND = auto()
    LOG_OR = auto()
    LOG_XOR = auto()
    ASSIGN = auto()
    REF_ASSIGN = auto()


@dataclass(frozen=True)
class TypeRef:
    parts: Tuple[str, ...]
    generics: Tuple["TypeRef", ...] = field(default_factory=tuple)

    def qualified_name(self) -> str:
        return ".".join(self.parts)


class Node:
    pass


class Statement(Node):
    pass


class Expression(Node):
    pass


@dataclass
class Program(Node):
    declarations: List["ClassDecl"]


@dataclass
class TemplateParameter(Node):
    name: str


@dataclass
class Parameter(Node):
    name: str
    type_ref: Optional[TypeRef]
    by_ref: bool = False
    default: Optional[Expression] = None


@dataclass
class FieldDecl(Node):
    name: str
    type_ref: Optional[TypeRef]
    scope: Scope
    shared: bool
    initializer: Optional[Expression]


@dataclass
class MethodDecl(Node):
    name: str
    scope: Scope
    shared: bool
    is_sub: bool
    return_type: Optional[TypeRef]
    parameters: List[Parameter]
    body: "Block"


@dataclass
class ClassDecl(Node):
    name: str
    scope: Scope
    class_kind: ClassKind
    shared: bool
    abstract: bool
    sealed: bool
    template_params: List[TemplateParameter]
    inherits: List[TypeRef]
    members: List["MemberDecl"]


@dataclass
class EventDecl(Node):
    name: str
    scope: Scope
    shared: bool
    parameters: List[Parameter]


MemberDecl = FieldDecl | MethodDecl | ClassDecl | EventDecl


@dataclass
class Block(Statement):
    statements: List[Statement]


@dataclass
class VarDecl(Statement):
    names: List[str]
    type_ref: Optional[TypeRef]
    initializer: Optional[Expression]


@dataclass
class ExprStmt(Statement):
    expr: Expression


@dataclass
class ReturnStmt(Statement):
    value: Optional[Expression]


@dataclass
class IfStmt(Statement):
    branches: List[Tuple[Expression, Block]]
    else_block: Optional[Block]


@dataclass
class ForStmt(Statement):
    target: Expression
    initializer: Expression
    direction: int  # +1 for To, -1 for DownTo
    limit: Expression
    step: Optional[Expression]
    body: Block


@dataclass
class Identifier(Expression):
    name: str


@dataclass
class Literal(Expression):
    value: object


@dataclass
class MemberAccess(Expression):
    obj: Expression
    member: str


@dataclass
class CallExpr(Expression):
    func: Expression
    args: List[Expression]


@dataclass
class BinaryOpExpr(Expression):
    left: Expression
    op: BinaryOperator
    right: Expression


@dataclass
class UnaryOpExpr(Expression):
    op: UnaryOperator
    operand: Expression


@dataclass
class AssignmentExpr(Expression):
    target: Expression
    value: Expression
    op: AssignmentOp


@dataclass
class NewExpr(Expression):
    type_ref: TypeRef
    args: List[Expression]


@dataclass
class CastExpr(Expression):
    target_type: TypeRef
    value: Expression
