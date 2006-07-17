#include "frbexpr.h"
#include "../common/assert.h"
#include "frbkeywords.h"

std::ostream& operator<<(std::ostream& s, const FrBExpr& expr)
{
    return expr.put(s);
}


/*        FrBBinOpExpr            */
FrBBinOpExpr::FrBBinOpExpr(FrBExpr* rhs, FrBExpr* lhs, int op)
    : _rhs(rhs), _lhs(lhs), _op(op)
{
    frb_assert2(rhs && lhs, "frbexpr.cpp::FrBOpExpr::FrBOpExpr()");
    
    /*try
    {
        _fn = lhs->getClass()->findOperator(op
    }
    catch(FrBFunctionNotFoundException)
    {
    }*/
}
FrBBinOpExpr::~FrBBinOpExpr()
{
    delete _rhs;
    delete _lhs;
}

FrBBaseObject* FrBBinOpExpr::eval() const throw (FrBEvaluationException)
{

    /*FrBBaseObject* lval = _rhs->eval();
    FrBBaseObject* rval = _lhs->eval();
    
    return lval->callOperator('+', */

}

const FrBClass* FrBBinOpExpr::getClass() const
{
    frb_assert2(_fn, "frbexpr.cpp::FrBOpExpr::getClass() - _fn is a null pointer");
    return _fn->returnType();
}

std::ostream& FrBBinOpExpr::put(std::ostream& stream) const
{
    return stream << '(' << *_rhs << ' ' << FrBKeywords::getKeywordOrSymbol(_op) << ' ' << *_lhs << ')';
}


/*          FrBLiteralExpr            */






