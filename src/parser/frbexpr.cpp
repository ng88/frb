#include "frbexpr.h"
#include "../common/assert.h"

std::ostream& operator<<(std::ostream& s, const FrBExpr& expr)
{
    return expr.put(s);
}


/*        FrBBinOpExpr            */
FrBBinOpExpr::FrBBinOpExpr(FrBExpr* rhs, FrBExpr* lhs, char* op)
    : _rhs(rhs), _lhs(lhs), _op(op)
{
    frb_assert2(rhs && lhs, "frbparsingtree.h::FrBOpExpr::FrBOpExpr()");
}
FrBBinOpExpr::~FrBBinOpExpr()
{
    delete _rhs;
    delete _lhs;
}

FrBBaseObject* FrBBinOpExpr::eval() const throw (FrBEvaluationException)
{
    /*
    FrBBaseObject* lval = _rhs->execute();
    FrBBaseObject* rval = _lhs->execute();
    
    return lval->callOperator('+', 
    */
    return NULL;
}

const FrBClass* FrBBinOpExpr::getClass() const
{
    return NULL;
}

std::ostream& FrBBinOpExpr::put(std::ostream& stream) const
{
    return stream << '(' << *_rhs << _op << *_lhs << ')';
}


/*          FrBLiteralExpr            */






