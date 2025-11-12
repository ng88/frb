from __future__ import annotations

from typing import Any

from src2.ast import nodes

from .base import NodeVisitor


class ConstantFolder(NodeVisitor):
    """Simple constant folding and block simplification."""

    def fold(self, program: nodes.Program) -> nodes.Program:
        self.visit(program)
        return program

    # ------------------------------------------------------------------ #
    def visit_Program(self, program: nodes.Program) -> nodes.Program:
        for decl in program.declarations:
            self.visit(decl)
        return program

    def visit_ClassDecl(self, decl: nodes.ClassDecl) -> nodes.ClassDecl:
        for member in decl.members:
            self.visit(member)
        return decl

    def visit_FieldDecl(self, field: nodes.FieldDecl) -> nodes.FieldDecl:
        if field.initializer:
            field.initializer = self._fold_expr(field.initializer)
        return field

    def visit_MethodDecl(self, method: nodes.MethodDecl) -> nodes.MethodDecl:
        self.visit(method.body)
        return method

    def visit_EventDecl(self, event: nodes.EventDecl) -> nodes.EventDecl:
        return event

    def visit_Block(self, block: nodes.Block) -> nodes.Block:
        new_statements: list[nodes.Statement] = []
        for stmt in block.statements:
            folded = self.visit(stmt)
            if isinstance(folded, nodes.Block):
                new_statements.extend(folded.statements)
            elif folded is not None:
                new_statements.append(folded)
        block.statements = new_statements
        return block

    def visit_VarDecl(self, decl: nodes.VarDecl) -> nodes.VarDecl:
        if decl.initializer:
            decl.initializer = self._fold_expr(decl.initializer)
        return decl

    def visit_ReturnStmt(self, stmt: nodes.ReturnStmt) -> nodes.ReturnStmt:
        if stmt.value:
            stmt.value = self._fold_expr(stmt.value)
        return stmt

    def visit_ExprStmt(self, stmt: nodes.ExprStmt) -> nodes.ExprStmt:
        stmt.expr = self._fold_expr(stmt.expr)
        return stmt

    def visit_IfStmt(self, stmt: nodes.IfStmt) -> nodes.Statement:
        new_branches = []
        for cond, block in stmt.branches:
            new_branches.append((self._fold_expr(cond), self.visit(block)))
        stmt.branches = new_branches
        if stmt.else_block:
            stmt.else_block = self.visit(stmt.else_block)
        simplified = self._simplify_if(stmt)
        return simplified

    def visit_ForStmt(self, stmt: nodes.ForStmt) -> nodes.ForStmt:
        stmt.initializer = self._fold_expr(stmt.initializer)
        stmt.limit = self._fold_expr(stmt.limit)
        if stmt.step:
            stmt.step = self._fold_expr(stmt.step)
        stmt.body = self.visit(stmt.body)
        return stmt

    # ------------------------------------------------------------------ #
    def _fold_expr(self, expr: nodes.Expression) -> nodes.Expression:
        if isinstance(expr, nodes.Literal):
            return expr
        if isinstance(expr, nodes.Identifier):
            return expr
        if isinstance(expr, nodes.AssignmentExpr):
            expr.value = self._fold_expr(expr.value)
            return expr
        if isinstance(expr, nodes.MemberAccess):
            expr.obj = self._fold_expr(expr.obj)
            return expr
        if isinstance(expr, nodes.CallExpr):
            expr.func = self._fold_expr(expr.func)
            expr.args = [self._fold_expr(arg) for arg in expr.args]
            return expr
        if isinstance(expr, nodes.NewExpr):
            expr.args = [self._fold_expr(arg) for arg in expr.args]
            return expr
        if isinstance(expr, nodes.CastExpr):
            expr.value = self._fold_expr(expr.value)
            return expr
        if isinstance(expr, nodes.BinaryOpExpr):
            expr.left = self._fold_expr(expr.left)
            expr.right = self._fold_expr(expr.right)
            result = self._eval_binary(expr.op, expr.left, expr.right)
            if result is not None:
                return nodes.Literal(result)
            return expr
        if isinstance(expr, nodes.UnaryOpExpr):
            expr.operand = self._fold_expr(expr.operand)
            result = self._eval_unary(expr.op, expr.operand)
            if result is not None:
                return nodes.Literal(result)
            return expr
        return expr

    def _eval_binary(self, op: nodes.BinaryOperator, left: nodes.Expression, right: nodes.Expression) -> Any | None:
        if not isinstance(left, nodes.Literal) or not isinstance(right, nodes.Literal):
            return None
        lval, rval = left.value, right.value
        try:
            if op is nodes.BinaryOperator.ADD:
                return lval + rval
            if op is nodes.BinaryOperator.SUB:
                return lval - rval
            if op is nodes.BinaryOperator.MUL:
                return lval * rval
            if op is nodes.BinaryOperator.DIV:
                return lval / rval
            if op is nodes.BinaryOperator.INT_DIV:
                return lval // rval
            if op is nodes.BinaryOperator.POW:
                return lval ** rval
            if op is nodes.BinaryOperator.MOD:
                return lval % rval
            if op is nodes.BinaryOperator.CONCAT:
                return str(lval) + str(rval)
            if op is nodes.BinaryOperator.EQ:
                return lval == rval
            if op is nodes.BinaryOperator.NE:
                return lval != rval
            if op is nodes.BinaryOperator.LT:
                return lval < rval
            if op is nodes.BinaryOperator.GT:
                return lval > rval
            if op is nodes.BinaryOperator.LE:
                return lval <= rval
            if op is nodes.BinaryOperator.GE:
                return lval >= rval
            if op is nodes.BinaryOperator.LOG_AND:
                return lval and rval
            if op is nodes.BinaryOperator.LOG_OR:
                return lval or rval
            if op is nodes.BinaryOperator.LOG_XOR:
                return bool(lval) ^ bool(rval)
        except Exception:
            return None
        return None

    def _eval_unary(self, op: nodes.UnaryOperator, operand: nodes.Expression) -> Any | None:
        if not isinstance(operand, nodes.Literal):
            return None
        try:
            if op is nodes.UnaryOperator.NEGATE:
                return -operand.value
            if op is nodes.UnaryOperator.NOT:
                return not operand.value
        except Exception:
            return None
        return None

    def _simplify_if(self, stmt: nodes.IfStmt) -> nodes.Statement:
        for cond, block in stmt.branches:
            if isinstance(cond, nodes.Literal):
                if cond.value:
                    return block
                continue
            break
        if stmt.else_block:
            return stmt
        if not stmt.branches:
            return nodes.Block([])
        return stmt
