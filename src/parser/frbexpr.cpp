/***************************************************************************
 *   This file is part of the FrB project                                  *
 *                                                                         *
 *   FrB project                                                           *
 *                                                                         *
 *   Copyright (C) 2006 by GUILLAUME Nicolas                               *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://frb.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   See the COPYING file.                                                 *
 ***************************************************************************/


#include "frbexpr.h"
#include "../common/assert.h"
#include "frbkeywords.h"
#include "frbfunction.h"
#include "frbresolveenvironment.h"


std::ostream& operator<<(std::ostream& s, const FrBExpr& expr)
{
    return expr.put(s);
}


/*         FrBLocalVarExpr        */

FrBLocalVarExpr::FrBLocalVarExpr(FrBTypeExpr * t, int varid)
 : _type(t), _varid(varid)
{
}

FrBLocalVarExpr::~FrBLocalVarExpr()
{
}

FrBBaseObject* FrBLocalVarExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    return e.stack().getTopValue(_varid);
}

void FrBLocalVarExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    _type->resolveAndCheck(e);
}

const FrBClass* FrBLocalVarExpr::getClass() const
{
    return _type->getClass();
}

std::ostream& FrBLocalVarExpr::put(std::ostream& stream) const
{
    return stream << "<local_var:" << _varid << ">";
}

/*     FrBUnresolvedIdExpr      */


FrBUnresolvedIdExpr::FrBUnresolvedIdExpr(const String& name)
 : _name(name), _type(0)
{
}

FrBUnresolvedIdExpr::~FrBUnresolvedIdExpr()
{
}

void FrBUnresolvedIdExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    _type = e.getClassFromName(_name);
}

FrBBaseObject* FrBUnresolvedIdExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    frb_assert2(_type, "_type is a null pointer"
                     " - FrBUnresolvedIdExpr::resolveAndCheck() probably not called");
    return 0;
}

const FrBClass* FrBUnresolvedIdExpr::getClass() const
{
    frb_assert2(_type, "_type is a null pointer"
                     " - FrBUnresolvedIdExpr::resolveAndCheck() probably not called");
    return _type;
}

std::ostream& FrBUnresolvedIdExpr::put(std::ostream& stream) const
{
    if(_type)
        return stream << "<" << _type->name() << ">";
    else
        return stream << "<unresolved:" << _name << "?>";
}





/*     FrBMemberOpExpr      */

FrBMemberOpExpr::FrBMemberOpExpr(FrBExpr* lhs, FrBUnresolvedIdExpr* rhs)
 : _lhs(lhs), _rhs(rhs)
{
    frb_assert2(rhs && lhs, "frbexpr.cpp::FrBMemberOpExpr::FrBMemberOpExpr()");
}

FrBMemberOpExpr::~FrBMemberOpExpr()
{
    delete _lhs;
    delete _rhs;
}

void FrBMemberOpExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    _lhs->resolveAndCheck(e);
    
    const FrBClass* cc  = _lhs->getClass();
    const String & name =  _rhs->name();
    
    
    /* _rhs est soit une inner classe soit un membre soit tout ce qui est function */
    
//     /*  function/sub */
//     FrBClass::FnPairIt pit = cc->findFunctions(name);
//     
//     if(pit.second->second)
//     {
//         /* ambiguity */
//     
//         puts("ID AMBIGUITY -- /* function/sub */\n");
//         exit(0);
//         
//         break;
//     }
//     else if(pit.first != cc->functionList()->end())
//     {
//         /* found */
//         
//         $<expr>$ = new FrBObjectIdExpr(new FrBFunctionWrapper( pit.first->second ));
//         
//         puts("ID FOUND -- /* 4. function/sub */\n");
//         
//         break;
//     }

   // throw FrBMemberNotFoundException(cc, name);
   
   _resolved = false;
    
}

FrBBaseObject* FrBMemberOpExpr::eval(FrBExecutionEnvironment&) const throw (FrBEvaluationException)
{
    return 0;
}

const FrBClass* FrBMemberOpExpr::getClass() const
{
    return 0;
}

std::ostream& FrBMemberOpExpr::put(std::ostream& stream) const
{
    return stream << '(' << *_lhs << ' '
        << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_MEMBER)
        << ' ' << _rhs->name() << ')';
}

/*              FrBFunctionCallExpr                */


FrBFunctionCallExpr::FrBFunctionCallExpr(FrBExpr* lhs, FrBExprList* rhs)
 : _lhs(lhs), _rhs(rhs), _fn(0)
{
    frb_assert2(rhs && lhs, "frbexpr.cpp::FrBFunctionCallExpr::FrBFunctionCallExpr()");
}

FrBFunctionCallExpr::~FrBFunctionCallExpr()
{
}

void FrBFunctionCallExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    _lhs->resolveAndCheck(e);
    
    for(FrBExprList::iterator it = _rhs->begin(); it != _rhs->end(); ++it)
        (*it)->resolveAndCheck(e);
        
    FrBMemberOpExpr * mo = dynamic_cast<FrBMemberOpExpr*>(_lhs);
    
    if(mo && !mo->resolved()) /* case of a real function call */
    {
        _fn = mo->lhs()->getClass()->findFunction(mo->rhs()->name(), *_rhs);
    }
    else /* case of an operator overload */
    {
     
    #if 1-1
        
        
        
        const FrBClass * cl = _lhs->getClass();
        const FrBClass * cr = _rhs->getClass();
        
        FrBConstClassList args;
        args.push_back(cl);
        args.push_back(cr);
        
        
        try
        {
            _fn = cl->findOperator(_op, args);
            if(!_fn->shared()) throw FrBFunctionNotFoundException("");
        }
        catch(FrBFunctionNotFoundException)
        {
            try
            {
                _fn = cr->findOperator(_op, args);
                if(!_fn->shared()) throw FrBFunctionNotFoundException("");
            }
            catch(FrBFunctionNotFoundException)
            {
                try
                {
                    args.clear();
                    args.push_back(cr);
                    _fn = cl->findOperator(_op, args);
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
    #endif
    }
}

FrBBaseObject* FrBFunctionCallExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    frb_assert2(_fn, "frbexpr.cpp::FrBFunctionCallExpr::val() - _fn is a null pointer");
    
    
    FrBBaseObjectList rval(_rhs->size());
    
    
    for(FrBExprList::iterator it = _rhs->begin(); it != _rhs->end(); ++it)
        rval.push_back((*it)->eval(e));
        
    FrBMemberOpExpr * mo = dynamic_cast<FrBMemberOpExpr*>(_lhs);
    
    if(mo && !mo->resolved()) /* case of a real function call */
    {
        FrBBaseObject* me = _fn->shared() ? 0 : mo->lhs()->eval(e);
        return _fn->execute(e, me, rval);
    }
    else
    {
        FrBBaseObject* lval = _lhs->eval(e);
        if(_fn->shared())
            return _fn->execute(e, 0, rval);
        else
            return _fn->execute(e, lval, rval);
    }
}

const FrBClass* FrBFunctionCallExpr::getClass() const
{
    return _lhs->getClass();
}

std::ostream& FrBFunctionCallExpr::put(std::ostream& stream) const
{
    stream << *_lhs << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_O_BRACKET);
    
    for(FrBExprList::const_iterator it = _rhs->begin(); it != _rhs->end(); ++it)
        stream << **it << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_LIST_SEP)  << ' ';
        
            
    return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_C_BRACKET);
}
 


  


/*        FrBBinOpExpr            */
FrBBinOpExpr::FrBBinOpExpr(FrBExpr* lhs, FrBExpr* rhs, int op) throw (FrBFunctionNotFoundException)
    : _rhs(rhs), _lhs(lhs), _op(op), _fn(0)
{
    frb_assert2(rhs && lhs, "frbexpr.cpp::FrBOpExpr::FrBOpExpr()");

}
FrBBinOpExpr::~FrBBinOpExpr()
{
    delete _rhs;
    delete _lhs;
}

void FrBBinOpExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
   
    _lhs->resolveAndCheck(e);
    _rhs->resolveAndCheck(e);
    
    const FrBClass * cl = _lhs->getClass();
    const FrBClass * cr = _rhs->getClass();
    
    FrBConstClassList args;
    args.push_back(cl);
    args.push_back(cr);
    
    
    try
    {
        _fn = cl->findOperator(_op, args);
        if(!_fn->shared()) throw FrBFunctionNotFoundException("");
    }
    catch(FrBFunctionNotFoundException)
    {
        try
        {
            _fn = cr->findOperator(_op, args);
            if(!_fn->shared()) throw FrBFunctionNotFoundException("");
        }
        catch(FrBFunctionNotFoundException)
        {
            try
            {
                args.clear();
                args.push_back(cr);
                _fn = cl->findOperator(_op, args);
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

FrBBaseObject* FrBBinOpExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{

    frb_assert2(_fn, "frbexpr.cpp::FrBOpExpr::val() - _fn is a null pointer");
    frb_assert2(_lhs, "frbexpr.cpp::FrBOpExpr::val() - _lhs is a null pointer");
    frb_assert2(_rhs, "frbexpr.cpp::FrBOpExpr::val() - _rhs is a null pointer");

    
    
    FrBBaseObject* lval = _lhs->eval(e);
    FrBBaseObject* rval = _rhs->eval(e);
    
    if(_fn->shared())
        return _fn->execute(e, 0, lval, rval);
    else
        return _fn->execute(e, lval, rval);
        

}

const FrBClass* FrBBinOpExpr::getClass() const
{
    frb_assert2(_fn, "frbexpr.cpp::FrBOpExpr::getClass() - _fn is a null pointer"
                     " - FrBBinOpExpr::resolveAndCheck() probably not called");
    return _fn->returnType();
}

std::ostream& FrBBinOpExpr::put(std::ostream& stream) const
{
    if(_fn)
        return stream << _fn->name() << '(' << *_lhs << ", " << *_rhs << ')';
    else
        return stream << '(' << *_lhs << " <unresolved:" << FrBKeywords::getKeywordOrSymbol(_op)
                << "> " << *_rhs << ')';
}


/*          FrBLiteralExpr            */






