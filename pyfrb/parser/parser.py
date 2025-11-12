from __future__ import annotations

from dataclasses import dataclass
from typing import List, Optional, Sequence

from src2.ast import nodes
from src2.lexer.lexer import Lexer, LexerConfig
from src2.lexer.tokens import Token, TokenKind
from src2.utils.keywords import KeywordRegistry

from .errors import ParseError


@dataclass
class ModifierState:
    scope: nodes.Scope = nodes.Scope.DEFAULT
    shared: bool = False
    abstract: bool = False
    sealed: bool = False


def parse_source(source: str, filename: str, registry: KeywordRegistry) -> nodes.Program:
    lexer = Lexer(source, filename, LexerConfig(registry))
    tokens = list(lexer.tokenize())
    parser = Parser(tokens, filename)
    return parser.parse_program()


class Parser:
    def __init__(self, tokens: Sequence[Token], filename: str) -> None:
        self.tokens = list(tokens)
        self.filename = filename
        self.pos = 0

    # ------------------------------------------------------------------ #
    def parse_program(self) -> nodes.Program:
        declarations: List[nodes.ClassDecl] = []
        self._skip_newlines()
        while not self._is(TokenKind.EOF):
            declarations.append(self._parse_class_decl())
            self._skip_newlines()
        return nodes.Program(declarations)

    # ------------------------------------------------------------------ #
    def _parse_class_decl(self, initial_modifiers: Optional[ModifierState] = None) -> nodes.ClassDecl:
        modifiers = initial_modifiers or self._parse_modifiers()

        kind_token = self._expect_keyword({"CLASS", "INTERFACE", "MODULE"})
        class_kind = {
            "CLASS": nodes.ClassKind.CLASS,
            "INTERFACE": nodes.ClassKind.INTERFACE,
            "MODULE": nodes.ClassKind.MODULE,
        }[kind_token]

        name = self._expect_identifier(allow_keyword_names=True)
        template_params = self._parse_template_params()
        self._expect_newline()

        inherits: List[nodes.TypeRef] = []
        members: List[nodes.MemberDecl] = []

        while not self._check_keyword("END"):
            if self._is(TokenKind.EOF):
                raise ParseError(f"Unexpected EOF inside class '{name}'.")
            if self._match_keyword("INHERITS"):
                inherits.extend(self._parse_inherits_list())
                self._expect_newline()
                continue
            if self._current.kind is TokenKind.NEWLINE:
                self._advance()
                continue
            members.append(self._parse_member())

        self._expect_keyword({"END"})
        end_kind = self._expect_keyword({"CLASS", "INTERFACE", "MODULE"})
        if end_kind != kind_token:
            raise ParseError(f"Expected 'End {kind_token.title()}' but found 'End {end_kind.title()}'.")
        self._expect_newline(optional=True)

        return nodes.ClassDecl(
            name=name,
            scope=modifiers.scope,
            class_kind=class_kind,
            shared=modifiers.shared,
            abstract=modifiers.abstract or class_kind is nodes.ClassKind.INTERFACE,
            sealed=modifiers.sealed,
            template_params=template_params,
            inherits=inherits,
            members=members,
        )

    def _parse_member(self) -> nodes.MemberDecl:
        modifiers = self._parse_modifiers()
        token = self._current

        if token.kind is TokenKind.KEYWORD:
            if token.value in {"CLASS", "INTERFACE", "MODULE"}:
                return self._parse_class_decl(modifiers)
            if token.value in {"FUNCTION", "SUB"}:
                return self._parse_method(modifiers)
            if token.value == "EVENT":
                return self._parse_event(modifiers)
            if token.value == "OPERATOR":
                return self._parse_operator(modifiers)

        return self._parse_field(modifiers)

    # ------------------------------------------------------------------ #
    def _parse_field(self, modifiers: ModifierState) -> nodes.FieldDecl:
        names = self._parse_identifier_list()
        self._expect_keyword({"AS"})
        type_ref = self._parse_type_ref()

        initializer = None
        if self._match_symbol("OP_ASSIGN_REF") or self._match_symbol("OP_ASSIGN_VAL"):
            initializer = self._parse_expression()
        self._expect_newline()

        # only one declaration supported for now
        if len(names) != 1:
            raise ParseError("Multiple field names per declaration are not supported yet.")

        first_name = names[0]

        return nodes.FieldDecl(
            name=first_name,
            type_ref=type_ref,
            scope=modifiers.scope,
            shared=modifiers.shared,
            initializer=initializer,
        )

    def _parse_event(self, modifiers: ModifierState) -> nodes.EventDecl:
        self._expect_keyword({"EVENT"})
        name = self._expect_identifier()
        parameters: List[nodes.Parameter] = []
        if self._match_symbol("OP_O_BRACKET"):
            parameters = self._parse_parameter_list_body()
        self._expect_newline()
        return nodes.EventDecl(name=name, scope=modifiers.scope, shared=modifiers.shared, parameters=parameters)

    def _parse_method(self, modifiers: ModifierState) -> nodes.MethodDecl:
        kind = self._expect_keyword({"FUNCTION", "SUB"})
        is_sub = kind == "SUB"
        name = self._parse_method_name()

        parameters = self._parse_parameter_list()
        return_type = None
        if not is_sub and self._match_keyword("AS"):
            return_type = self._parse_type_ref()
        elif is_sub and self._match_keyword("AS"):
            raise ParseError("Sub procedures cannot declare a return type.")

        self._expect_newline()

        body_statements: List[nodes.Statement] = []
        while not (self._check_keyword("END") and self._check_keyword({"FUNCTION", "SUB"}, lookahead=1)):
            if self._is(TokenKind.EOF):
                raise ParseError(f"Unexpected EOF inside method '{name}'.")
            statement = self._parse_statement()
            if statement:
                body_statements.append(statement)

        self._expect_keyword({"END"})
        end_kind = self._expect_keyword({"FUNCTION", "SUB"})
        if end_kind != kind:
            raise ParseError(f"Mismatched terminator. Expected 'End {kind.title()}' got 'End {end_kind.title()}'.")
        self._expect_newline(optional=True)

        return nodes.MethodDecl(
            name=name,
            scope=modifiers.scope,
            shared=modifiers.shared,
            is_sub=is_sub,
            return_type=return_type,
            parameters=parameters,
            body=nodes.Block(body_statements),
        )

    def _parse_operator(self, modifiers: ModifierState) -> nodes.MethodDecl:
        self._expect_keyword({"OPERATOR"})
        operator_name = self._parse_operator_symbol()
        parameters = self._parse_parameter_list()
        self._expect_keyword({"AS"})
        return_type = self._parse_type_ref()
        self._expect_newline()

        body_statements: List[nodes.Statement] = []
        while not (self._check_keyword("END") and self._check_keyword({"OPERATOR"}, lookahead=1)):
            body_statements.append(self._parse_statement())

        self._expect_keyword({"END"})
        self._expect_keyword({"OPERATOR"})
        self._expect_newline(optional=True)

        return nodes.MethodDecl(
            name=f"operator{operator_name}",
            scope=modifiers.scope,
            shared=modifiers.shared,
            is_sub=False,
            return_type=return_type,
            parameters=parameters,
            body=nodes.Block(body_statements),
        )

    # ------------------------------------------------------------------ #
    def _parse_statement(self) -> Optional[nodes.Statement]:
        self._skip_newlines()
        token = self._current
        if token.kind is TokenKind.EOF:
            return None

        if token.kind is TokenKind.KEYWORD:
            if token.value == "DECLARE" or token.value == "DIM":
                return self._parse_var_decl()
            if token.value == "IF":
                return self._parse_if()
            if token.value == "FOR":
                return self._parse_for()
            if token.value == "RETURN":
                return self._parse_return()

        expr = self._parse_expression()
        self._expect_newline()
        return nodes.ExprStmt(expr)

    def _parse_var_decl(self) -> nodes.Statement:
        self._advance()  # consume Dim/Declare
        decls: List[nodes.VarDecl] = []
        while True:
            names = self._parse_identifier_list()
            self._expect_keyword({"AS"})
            type_ref = self._parse_type_ref()
            initializer = None
            if self._match_symbol("OP_ASSIGN_REF") or self._match_symbol("OP_ASSIGN_VAL"):
                initializer = self._parse_expression()
            decls.append(nodes.VarDecl(names=names, type_ref=type_ref, initializer=initializer))
            if not self._match_symbol("OP_LIST_SEP"):
                break
        self._expect_newline()
        if len(decls) == 1:
            return decls[0]
        return nodes.Block(decls)

    def _parse_if(self) -> nodes.IfStmt:
        self._expect_keyword({"IF"})
        branches: List[tuple[nodes.Expression, nodes.Block]] = []

        def parse_branch() -> nodes.Block:
            stmts: List[nodes.Statement] = []
            while True:
                if self._check_keyword("END"):
                    break
                if self._check_keyword("ELSE") and self._check_keyword({"IF"}, lookahead=1):
                    break
                if self._check_keyword("ELSE"):
                    break
                stmts.append(self._parse_statement())
            return nodes.Block(stmts)

        cond = self._parse_expression()
        self._expect_keyword({"THEN"})

        if self._current.kind is not TokenKind.NEWLINE:
            then_stmt = self._parse_statement()
            branches.append((cond, nodes.Block([then_stmt])))
            else_block = None
            if self._match_keyword("ELSE"):
                else_stmt = self._parse_statement()
                else_block = nodes.Block([else_stmt])
            return nodes.IfStmt(branches=branches, else_block=else_block)

        self._expect_newline()
        branches.append((cond, parse_branch()))

        while self._check_keyword("ELSE") and self._check_keyword({"IF"}, lookahead=1):
            self._expect_keyword({"ELSE"})
            self._expect_keyword({"IF"})
            cond = self._parse_expression()
            self._expect_keyword({"THEN"})
            self._expect_newline()
            branches.append((cond, parse_branch()))

        else_block = None
        if self._match_keyword("ELSE"):
            self._expect_newline()
            stmts: List[nodes.Statement] = []
            while not self._check_keyword("END"):
                stmts.append(self._parse_statement())
            else_block = nodes.Block(stmts)

        self._expect_keyword({"END"})
        self._expect_keyword({"IF"})
        self._expect_newline()

        return nodes.IfStmt(branches=branches, else_block=else_block)

    def _parse_for(self) -> nodes.ForStmt:
        self._expect_keyword({"FOR"})
        target = self._parse_assignable_expression()
        assign_op = self._expect_assignment_operator()
        init_expr = self._parse_expression()

        if self._match_keyword("TO"):
            direction = 1
        elif self._match_keyword("DOWNTO"):
            direction = -1
        else:
            raise ParseError("Expected 'To' or 'DownTo' in For loop.")
        limit_expr = self._parse_expression()

        step_expr = None
        if self._match_keyword("STEP"):
            step_expr = self._parse_expression()

        self._expect_newline()
        body_statements: List[nodes.Statement] = []
        while not self._check_keyword("NEXT"):
            body_statements.append(self._parse_statement())

        self._expect_keyword({"NEXT"})
        if self._current.kind is TokenKind.IDENTIFIER:
            self._advance()  # consume optional iterator name
        self._expect_newline(optional=True)

        return nodes.ForStmt(
            target=target,
            initializer=nodes.AssignmentExpr(target, init_expr, assign_op),
            direction=direction,
            limit=limit_expr,
            step=step_expr,
            body=nodes.Block(body_statements),
        )

    def _parse_return(self) -> nodes.ReturnStmt:
        self._expect_keyword({"RETURN"})
        if self._current.kind in (TokenKind.NEWLINE, TokenKind.EOF):
            self._expect_newline()
            return nodes.ReturnStmt(None)
        expr = self._parse_expression()
        self._expect_newline()
        return nodes.ReturnStmt(expr)

    # ------------------------------------------------------------------ #
    # expressions
    # ------------------------------------------------------------------ #
    def _parse_expression(self) -> nodes.Expression:
        return self._parse_assignment_expr()

    def _parse_assignment_expr(self) -> nodes.Expression:
        expr = self._parse_logical_or()
        while True:
            op = self._match_assignment_operator()
            if not op:
                break
            value = self._parse_assignment_expr()
            expr = nodes.AssignmentExpr(expr, value, op)
        return expr

    def _parse_logical_or(self) -> nodes.Expression:
        expr = self._parse_logical_and()
        while True:
            if self._match_keyword("OP_LOG_OR"):
                right = self._parse_logical_and()
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.LOG_OR, right)
                continue
            if self._match_keyword("OP_LOG_XOR"):
                right = self._parse_logical_and()
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.LOG_XOR, right)
                continue
            break
        return expr

    def _parse_logical_and(self) -> nodes.Expression:
        expr = self._parse_comparison()
        while self._match_keyword("OP_LOG_AND"):
            right = self._parse_comparison()
            expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.LOG_AND, right)
        return expr

    def _parse_comparison(self) -> nodes.Expression:
        expr = self._parse_concat()
        while True:
            if self._match_symbol("OP_EQ"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.EQ, self._parse_concat())
            elif self._match_symbol("OP_NE"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.NE, self._parse_concat())
            elif self._match_symbol("OP_LT"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.LT, self._parse_concat())
            elif self._match_symbol("OP_GT"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.GT, self._parse_concat())
            elif self._match_symbol("OP_LE"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.LE, self._parse_concat())
            elif self._match_symbol("OP_GE"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.GE, self._parse_concat())
            elif self._match_keyword("OP_IS"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.IS, self._parse_concat())
            elif self._match_keyword("OP_IN"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.IN, self._parse_concat())
            else:
                break
        return expr

    def _parse_concat(self) -> nodes.Expression:
        expr = self._parse_additive()
        while self._match_symbol("OP_CONCAT"):
            right = self._parse_additive()
            expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.CONCAT, right)
        return expr

    def _parse_additive(self) -> nodes.Expression:
        expr = self._parse_multiplicative()
        while True:
            if self._match_symbol("OP_ADD"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.ADD, self._parse_multiplicative())
            elif self._match_symbol("OP_SUB"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.SUB, self._parse_multiplicative())
            else:
                break
        return expr

    def _parse_multiplicative(self) -> nodes.Expression:
        expr = self._parse_power()
        while True:
            if self._match_symbol("OP_MUL"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.MUL, self._parse_power())
            elif self._match_symbol("OP_DIV"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.DIV, self._parse_power())
            elif self._match_symbol("OP_INT_DIV"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.INT_DIV, self._parse_power())
            elif self._match_keyword("OP_MOD") or self._match_symbol("OP_MOD"):
                expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.MOD, self._parse_power())
            else:
                break
        return expr

    def _parse_power(self) -> nodes.Expression:
        expr = self._parse_unary()
        if self._match_symbol("OP_POW"):
            expr = nodes.BinaryOpExpr(expr, nodes.BinaryOperator.POW, self._parse_power())
        return expr

    def _parse_unary(self) -> nodes.Expression:
        if self._match_symbol("OP_SUB"):
            return nodes.UnaryOpExpr(nodes.UnaryOperator.NEGATE, self._parse_unary())
        if self._match_keyword("OP_LOG_NOT"):
            return nodes.UnaryOpExpr(nodes.UnaryOperator.NOT, self._parse_unary())
        if self._match_symbol("OP_CARD"):
            return nodes.UnaryOpExpr(nodes.UnaryOperator.CARDINAL, self._parse_unary())
        return self._parse_postfix()

    def _parse_postfix(self) -> nodes.Expression:
        expr = self._parse_primary()
        while True:
            if self._match_symbol("OP_MEMBER"):
                member_name = self._expect_identifier(allow_keyword_names=True)
                expr = nodes.MemberAccess(expr, member_name)
                continue
            if self._match_symbol("OP_O_BRACKET"):
                args = self._parse_argument_list()
                expr = nodes.CallExpr(expr, args)
                continue
            if self._match_symbol("OP_INCR"):
                expr = nodes.UnaryOpExpr(nodes.UnaryOperator.POST_INC, expr)
                continue
            if self._match_symbol("OP_DECR"):
                expr = nodes.UnaryOpExpr(nodes.UnaryOperator.POST_DEC, expr)
                continue
            break
        return expr

    def _parse_primary(self) -> nodes.Expression:
        token = self._current
        if token.kind is TokenKind.INT:
            self._advance()
            return nodes.Literal(token.value)
        if token.kind is TokenKind.FLOAT:
            self._advance()
            return nodes.Literal(token.value)
        if token.kind is TokenKind.STRING:
            self._advance()
            return nodes.Literal(token.value)
        if token.kind is TokenKind.IDENTIFIER:
            self._advance()
            return nodes.Identifier(token.value)
        if token.kind is TokenKind.KEYWORD:
            if token.value == "TRUE":
                self._advance()
                return nodes.Literal(True)
            if token.value == "FALSE":
                self._advance()
                return nodes.Literal(False)
            if token.value == "NULL":
                self._advance()
                return nodes.Literal(None)
            if token.value == "ME":
                self._advance()
                return nodes.Identifier("Me")
            if token.value == "OP_NEW":
                self._advance()
                type_ref = self._parse_type_ref()
                args: List[nodes.Expression] = []
                if self._match_symbol("OP_O_BRACKET"):
                    args = self._parse_argument_list()
                return nodes.NewExpr(type_ref, args)
            if token.value == "OP_CAST":
                self._advance()
                self._expect_symbol("OP_O_BRACKET")
                target_type = self._parse_type_ref()
                self._expect_symbol("OP_LIST_SEP")
                value_expr = self._parse_expression()
                self._expect_symbol("OP_C_BRACKET")
                return nodes.CastExpr(target_type, value_expr)
        if token.kind is TokenKind.SYMBOL and token.value == "OP_O_BRACKET":
            self._advance()
            expr = self._parse_expression()
            self._expect_symbol("OP_C_BRACKET")
            return expr
        raise ParseError(f"Unexpected token '{token.text}' in expression.")

    def _parse_argument_list(self) -> List[nodes.Expression]:
        args: List[nodes.Expression] = []
        if self._match_symbol("OP_C_BRACKET"):
            return args
        while True:
            args.append(self._parse_expression())
            if self._match_symbol("OP_C_BRACKET"):
                break
            self._expect_symbol("OP_LIST_SEP")
        return args

    def _parse_assignable_expression(self) -> nodes.Expression:
        expr = self._parse_postfix()
        return expr

    def _expect_assignment_operator(self) -> nodes.AssignmentOp:
        op = self._match_assignment_operator()
        if not op:
            raise ParseError("Expected assignment operator.")
        return op

    def _match_assignment_operator(self) -> Optional[nodes.AssignmentOp]:
        if self._match_symbol("OP_ASSIGN_REF"):
            return nodes.AssignmentOp.REF_ASSIGN
        if self._match_symbol("OP_ASSIGN_VAL"):
            return nodes.AssignmentOp.ASSIGN
        if self._match_symbol("OP_ADD_ASSIGN"):
            return nodes.AssignmentOp.ADD_ASSIGN
        if self._match_symbol("OP_SUB_ASSIGN"):
            return nodes.AssignmentOp.SUB_ASSIGN
        if self._match_symbol("OP_MUL_ASSIGN"):
            return nodes.AssignmentOp.MUL_ASSIGN
        if self._match_symbol("OP_DIV_ASSIGN"):
            return nodes.AssignmentOp.DIV_ASSIGN
        if self._match_symbol("OP_INT_DIV_ASSIGN"):
            return nodes.AssignmentOp.INT_DIV_ASSIGN
        if self._match_symbol("OP_POW_ASSIGN"):
            return nodes.AssignmentOp.POW_ASSIGN
        return None

    # ------------------------------------------------------------------ #
    # helpers
    # ------------------------------------------------------------------ #
    def _parse_modifiers(self) -> ModifierState:
        modifiers = ModifierState()
        consumed = True
        while consumed:
            consumed = False
            token = self._current
            if token.kind is TokenKind.KEYWORD:
                if token.value == "PUBLIC":
                    modifiers.scope = nodes.Scope.PUBLIC
                    consumed = True
                elif token.value == "PRIVATE":
                    modifiers.scope = nodes.Scope.PRIVATE
                    consumed = True
                elif token.value == "PROTECTED":
                    modifiers.scope = nodes.Scope.PROTECTED
                    consumed = True
                elif token.value == "FRIEND":
                    modifiers.scope = nodes.Scope.FRIEND
                    consumed = True
                elif token.value == "SHARED":
                    modifiers.shared = True
                    consumed = True
                elif token.value == "ABSTRACT":
                    modifiers.abstract = True
                    consumed = True
                elif token.value == "SEALED":
                    modifiers.sealed = True
                    consumed = True
                else:
                    consumed = False
            if consumed:
                self._advance()
        return modifiers

    def _parse_template_params(self) -> List[nodes.TemplateParameter]:
        params: List[nodes.TemplateParameter] = []
        if not self._match_symbol("OP_LT"):
            return params
        params.append(nodes.TemplateParameter(self._expect_identifier()))
        while self._match_symbol("OP_LIST_SEP"):
            params.append(nodes.TemplateParameter(self._expect_identifier()))
        self._expect_symbol("OP_GT")
        return params

    def _parse_type_ref(self) -> nodes.TypeRef:
        parts = [self._expect_identifier(allow_keyword_names=True)]
        while self._match_symbol("OP_MEMBER"):
            parts.append(self._expect_identifier(allow_keyword_names=True))
        generics: List[nodes.TypeRef] = []
        if self._match_symbol("OP_LT"):
            generics.append(self._parse_type_ref())
            while self._match_symbol("OP_LIST_SEP"):
                generics.append(self._parse_type_ref())
            self._expect_symbol("OP_GT")
        return nodes.TypeRef(tuple(parts), tuple(generics))

    def _parse_inherits_list(self) -> List[nodes.TypeRef]:
        types = [self._parse_type_ref()]
        while self._match_symbol("OP_LIST_SEP"):
            types.append(self._parse_type_ref())
        return types

    def _parse_method_name(self) -> str:
        token = self._current
        if token.kind is TokenKind.IDENTIFIER:
            self._advance()
            return token.value
        if token.kind is TokenKind.KEYWORD and token.value in {"CONSTRUCTOR_NAME", "DESTRUCTOR_NAME"}:
            self._advance()
            return token.text
        raise ParseError("Expected method name.")

    def _parse_operator_symbol(self) -> str:
        token = self._current
        if token.kind is TokenKind.SYMBOL:
            if token.value == "OP_O_BRACKET" and self._check_symbol("OP_C_BRACKET", lookahead=1):
                self._advance()
                self._advance()
                return "()"
            if token.value == "OP_ARRAY_SUB_BEGIN" and self._check_symbol("OP_ARRAY_SUB_END", lookahead=1):
                self._advance()
                self._advance()
                return "[]"
            self._advance()
            return token.text
        raise ParseError("Unsupported operator symbol.")

    def _parse_parameter_list(self) -> List[nodes.Parameter]:
        self._expect_symbol("OP_O_BRACKET")
        return self._parse_parameter_list_body()

    def _parse_parameter_list_body(self) -> List[nodes.Parameter]:
        params: List[nodes.Parameter] = []
        if self._match_symbol("OP_C_BRACKET"):
            return params
        while True:
            by_ref = False
            if self._match_keyword("BYREF"):
                by_ref = True
            elif self._match_keyword("BYVAL"):
                by_ref = False
            name = self._expect_identifier()
            type_ref = None
            if self._match_keyword("AS"):
                type_ref = self._parse_type_ref()
            default = None
            if self._match_symbol("OP_ASSIGN_REF") or self._match_symbol("OP_ASSIGN_VAL"):
                default = self._parse_expression()
            params.append(nodes.Parameter(name=name, type_ref=type_ref, by_ref=by_ref, default=default))

            if self._match_symbol("OP_LIST_SEP"):
                continue
            self._expect_symbol("OP_C_BRACKET")
            break
        return params

    def _parse_identifier_list(self) -> List[str]:
        names = [self._expect_identifier()]
        while self._match_symbol("OP_LIST_SEP"):
            names.append(self._expect_identifier())
        return names

    def _expect_identifier(self, allow_keyword_names: bool = False) -> str:
        token = self._current
        if token.kind is TokenKind.IDENTIFIER:
            self._advance()
            return token.value
        if allow_keyword_names and token.kind is TokenKind.KEYWORD:
            self._advance()
            return token.text
        raise ParseError(f"Expected identifier, got '{token.text}'.")

    def _expect_symbol(self, name: str) -> None:
        if self._match_symbol(name):
            return
        raise ParseError(f"Expected symbol '{name}'.")

    def _match_symbol(self, name: str) -> bool:
        token = self._current
        if token.kind is TokenKind.SYMBOL and token.value == name:
            self._advance()
            return True
        return False

    def _check_symbol(self, name: str, lookahead: int = 0) -> bool:
        token = self._peek(lookahead)
        return token.kind is TokenKind.SYMBOL and token.value == name

    def _expect_keyword(self, names: set[str]) -> str:
        token = self._current
        if token.kind is TokenKind.KEYWORD and token.value in names:
            self._advance()
            return token.value
        raise ParseError(f"Expected keyword {'/'.join(names)}.")

    def _match_keyword(self, name: str) -> bool:
        token = self._current
        if token.kind is TokenKind.KEYWORD and token.value == name:
            self._advance()
            return True
        return False

    def _check_keyword(self, name: str | set[str], lookahead: int = 0) -> bool:
        token = self._peek(lookahead)
        if token.kind is not TokenKind.KEYWORD:
            return False
        if isinstance(name, set):
            return token.value in name
        return token.value == name

    def _expect_newline(self, optional: bool = False) -> None:
        if optional:
            while self._match_newline():
                pass
            return
        if not self._match_newline():
            raise ParseError("Expected newline.")
        while self._match_newline():
            pass

    def _match_newline(self) -> bool:
        if self._current.kind is TokenKind.NEWLINE:
            self._advance()
            return True
        return False

    def _skip_newlines(self) -> None:
        while self._match_newline():
            continue

    def _expect(self, kind: TokenKind) -> Token:
        token = self._current
        if token.kind is not kind:
            raise ParseError(f"Expected {kind}, got {token.kind}.")
        self._advance()
        return token

    def _current_token(self) -> Token:
        return self._peek(0)

    @property
    def _current(self) -> Token:
        return self._peek(0)

    def _peek(self, offset: int) -> Token:

        idx = self.pos + offset
        if idx >= len(self.tokens):
            return self.tokens[-1]
        return self.tokens[idx]

    def _advance(self) -> Token:
        token = self._current
        if self.pos < len(self.tokens):
            self.pos += 1
        return token

    def _is(self, kind: TokenKind) -> bool:
        return self._current.kind is kind
