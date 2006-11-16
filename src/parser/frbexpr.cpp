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

/*          FrBExpr          */

std::ostream& operator<<(std::ostream& s, const FrBExpr& expr)
{
    return expr.put(s);
}

bool FrBExpr::isAssignable() const
{
    return false;
}

void FrBExpr::refAssign(FrBExecutionEnvironment&, FrBBaseObject* o) const throw (FrBEvaluationException)
{
    //frb_assert(isAssignable());
    frb_assert(o);

    throw FrBInvalidLValueException(this);
}


/*         FrBLocalVarExpr        */

FrBLocalVarExpr::FrBLocalVarExpr(FrBCodeFunction * fn, FrBTypeExpr * t, int varid)
 : _fn(fn), _type(t), _varid(varid)
{
}

FrBLocalVarExpr::~FrBLocalVarExpr()
{
}

bool FrBLocalVarExpr::isAssignable() const
{
    return true;
}

void FrBLocalVarExpr::refAssign(FrBExecutionEnvironment& e, FrBBaseObject* o)
    const throw (FrBEvaluationException)
{
     e.stack().setTopValue(_varid, o);
}

FrBBaseObject* FrBLocalVarExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    return e.stack().getTopValue(_varid);
}

void FrBLocalVarExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
  _varid += _fn->localVarCount();

  //already done by resolvePrototype()
  //_type->resolveAndCheck(e);
}

const FrBClass* FrBLocalVarExpr::getClass() const
{
    return _type->getClass();
}

std::ostream& FrBLocalVarExpr::put(std::ostream& stream) const
{
    return stream << "local_var" << _varid;
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
        return stream << _type->name();
    else
        return stream << "unresolved_" << _name;
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
    
    (void)cc;
    (void)name;
    
    //TODO a completer
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
    return stream << '(' << *_lhs
        << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_MEMBER)
        << _rhs->name() << ')';
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
    
    
    FrBBaseObjectList rval;
    rval.reserve(_rhs->size());

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
    frb_assert(_fn);
    return _fn->returnType();
}

std::ostream& FrBFunctionCallExpr::put(std::ostream& stream) const
{
    stream << *_lhs << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_O_BRACKET);

    FrBExprList::const_iterator it = _rhs->begin();

    if( it != _rhs->end() )
    {
      stream << **it;
      ++it;
    }

    while(it != _rhs->end())
    {
      stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_LIST_SEP) 
	     << ' ' << **it;
      ++it;
    }
        
            
    return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_C_BRACKET);
}
 
/*        FrBMeExpr                */

FrBMeExpr::FrBMeExpr(FrBCodeFunction* f)
 : _fn(f)
{
}

FrBMeExpr::~FrBMeExpr()
{
}

void FrBMeExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
  _varid = _fn->localVarCount();
}


FrBBaseObject* FrBMeExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    return e.stack().getTopValue(_varid);
}

const FrBClass* FrBMeExpr::getClass() const
{
    return _fn->container();
}

std::ostream& FrBMeExpr::put(std::ostream& stream) const
{
    return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_ME);
}

/*              FrBOutisdeMeExpr           */
FrBOutsideMeExpr::FrBOutsideMeExpr(FrBClass * t)
  : _type(t)
{
}

FrBOutsideMeExpr::~FrBOutsideMeExpr()
{
}

FrBBaseObject* FrBOutsideMeExpr::eval(FrBExecutionEnvironment& e) const
  throw (FrBEvaluationException)
{
  return e.outsideMe();
}

const FrBClass* FrBOutsideMeExpr::getClass() const
{
  return _type;
}

std::ostream& FrBOutsideMeExpr::put(std::ostream& stream) const
{
    return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_ME);
}



/*           FrBRefAssignExpr                    */


FrBRefAssignExpr::FrBRefAssignExpr(FrBExpr* lhs, FrBExpr* rhs) throw (FrBFunctionNotFoundException)
 : _lhs(lhs), _rhs(rhs)
{
    frb_assert(_lhs && _rhs);
}

FrBRefAssignExpr::~FrBRefAssignExpr()
{
}


void FrBRefAssignExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{

  _rhs->resolveAndCheck(e);
  _lhs->resolveAndCheck(e);
  
  if(!_rhs->getClass()->isCompatibleWith(_rhs->getClass()))
    throw FrBIncompatibleClassException(_rhs->getClass(), _rhs->getClass());
}

bool FrBRefAssignExpr::isAssignable() const
{
    return _lhs->isAssignable() && _rhs->isAssignable();
}

void FrBRefAssignExpr::refAssign(FrBExecutionEnvironment& e, FrBBaseObject* o) const
        throw (FrBEvaluationException)
{
    _rhs->refAssign(e, o);
}

FrBBaseObject* FrBRefAssignExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
  //TODO foreConvert() a mettre qqpart
    FrBBaseObject * r = _rhs->eval(e);
    
    e.memory()->delLink(_lhs->eval(e));
    e.memory()->addLink(r);
    
    _lhs->refAssign(e, r);
    
    return r;
    
}

const FrBClass* FrBRefAssignExpr::getClass() const
{
    return _lhs->getClass();
}

std::ostream& FrBRefAssignExpr::put(std::ostream& stream) const
{
    return stream << '(' << *_lhs << ' '
        << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_ASSIGN_REF)
        << ' ' << *_rhs << ')';
}



  


/*        FrBBinOpExpr            */
FrBBinOpExpr::FrBBinOpExpr(FrBExpr* lhs, FrBExpr* rhs, int op)
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
      //return stream << _fn->name() << '(' << *_lhs << ", " << *_rhs << ')';
      return stream << '(' << *_lhs << ' '
		    << FrBKeywords::getKeywordOrSymbol(_op) << ' '
		    << *_rhs << ')';
    else
        return stream << '(' << *_lhs << " <unresolved:" << FrBKeywords::getKeywordOrSymbol(_op)
                << "> " << *_rhs << ')';
}


/*          FrBLiteralExpr            */



std::ostream& FrBIntExpr::put(std::ostream& stream) const
{
  return stream << _value;
}

std::ostream& FrBBoolExpr::put(std::ostream& stream) const
{
  if(_value)
    return stream << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_TRUE);
  else
    return stream << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_FALSE);
}

std::ostream& FrBStringExpr::put(std::ostream& stream) const
{
  return stream << '\"' << _value << '\"';
}



/*         FrBNullExpr                */

FrBNullExpr * FrBNullExpr::_null_expr = NULL;
    
FrBNullExpr::FrBNullExpr()
{
}


FrBNullExpr::~FrBNullExpr()
{
}

FrBNullExpr * FrBNullExpr::nullExpr()
{
    if(!_null_expr)
        _null_expr = new FrBNullExpr();
        
    return _null_expr;
}

FrBBaseObject* FrBNullExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    return FrBNull::nullValue();
}

const FrBClass* FrBNullExpr::getClass() const
{
    return FrBNull::getCppClass();
}

std::ostream& FrBNullExpr::put(std::ostream& stream) const
{
    return stream << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_NULL);
}    


