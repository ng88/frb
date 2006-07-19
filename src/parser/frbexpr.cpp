#include "frbexpr.h"
#include "../common/assert.h"
#include "frbkeywords.h"

std::ostream& operator<<(std::ostream& s, const FrBExpr& expr)
{
    return expr.put(s);
}



/*        FrBIdExpr            */

FrBIdExpr::FrBIdExpr(const String& str) throw (FrBUndeclaredIdentifierException)
{
    //TODO
}

FrBIdExpr::~FrBIdExpr()
{
}


FrBBaseObject* FrBIdExpr::eval() const throw (FrBEvaluationException)
{
    return _wrapper;
}

const FrBClass* FrBIdExpr::getClass() const
{
    frb_assert2(_wrapper, "frbexpr.cpp::FrBIdExpr::getClass() - _wrapper is a null pointer");
    
    return _wrapper->getClass();
}

std::ostream& FrBIdExpr::put(std::ostream& stream) const
{
    return stream << "<identifier>";
}


/*        FrBBinOpExpr            */
FrBBinOpExpr::FrBBinOpExpr(FrBExpr* lhs, FrBExpr* rhs, int op) throw (FrBFunctionNotFoundException)
    : _rhs(rhs), _lhs(lhs), _op(op)
{
    frb_assert2(rhs && lhs, "frbexpr.cpp::FrBOpExpr::FrBOpExpr()");
    
    const FrBClass * cl = lhs->getClass();
    const FrBClass * cr = rhs->getClass();
    
    FrBConstClassList args;
    args.push_back(cl);
    args.push_back(cr);
    
    
    try
    {
        _fn = cl->findOperator(op, args);
        if(!_fn->shared()) throw FrBFunctionNotFoundException("");
    }
    catch(FrBFunctionNotFoundException)
    {
        try
        {
            _fn = cr->findOperator(op, args);
            if(!_fn->shared()) throw FrBFunctionNotFoundException("");
        }
        catch(FrBFunctionNotFoundException)
        {
            try
            {
                args.clear();
                args.push_back(cr);
                _fn = cl->findOperator(op, args);
                if(_fn->shared()) throw FrBFunctionNotFoundException("");
            }
            catch(FrBFunctionNotFoundException)
            {
                FrBFunctionNotFoundException ex(
                                String(FrBKeywords::getKeyword(FrBKeywords::FRB_KW_OPERATOR))
                                    .append(FrBKeywords::getKeywordOrSymbol(_op)),
                                args
                                                );
                
                const FrBClass::OperatorContainer * ops = cl->operatorList();
                
                for(FrBClass::OperatorContainer::const_iterator itf = ops->begin(); itf != ops->end(); ++itf)
                        ex.addCandidate(itf->second);
                        
                ops = cr->operatorList();
                
                for(FrBClass::OperatorContainer::const_iterator itf = ops->begin(); itf != ops->end(); ++itf)
                        ex.addCandidate(itf->second);
                                                                       
                throw ex;
            }
        }
    }
}
FrBBinOpExpr::~FrBBinOpExpr()
{
    delete _rhs;
    delete _lhs;
}

FrBBaseObject* FrBBinOpExpr::eval() const throw (FrBEvaluationException)
{

    frb_assert2(_fn, "frbexpr.cpp::FrBOpExpr::val() - _fn is a null pointer");
    frb_assert2(_lhs, "frbexpr.cpp::FrBOpExpr::val() - _lhs is a null pointer");
    frb_assert2(_rhs, "frbexpr.cpp::FrBOpExpr::val() - _rhs is a null pointer");

    
    
    FrBBaseObject* lval = _lhs->eval();
    FrBBaseObject* rval = _rhs->eval();
    
    if(_fn->shared())
        return _fn->execute(0, lval, rval);
    else
        return _fn->execute(lval, rval);
        

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






