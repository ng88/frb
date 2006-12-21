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
#include "frbbuiltinclasses.h"

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
    delete _type;
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

/*     FrBUnresolvedTypeExpr      */


FrBUnresolvedTypeExpr::FrBUnresolvedTypeExpr(const String& name, FrBCodeClass* current,
                                                        FrBUnresolvedTypeExpr * ctxt)
 : _name(name), _type(0), _context(ctxt), _currentClass(current)
{
}

FrBUnresolvedTypeExpr::~FrBUnresolvedTypeExpr()
{
    //if(_context)
        //delete _context;
}

void FrBUnresolvedTypeExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    if(_context)
    {
        _context->resolveAndCheck(e);
        _type = e.getNextClassFromName(_name, _context->getContext());
        
        delete _context; /* we don't need _context anymore */
        _context = 0;
    }
    else
    {
        _type = e.getClassFromName(_name, _currentClass);
    }
        
    
}

FrBBaseObject* FrBUnresolvedTypeExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    frb_assert2(false, "can't eval FrBUnresolvedTypeExpr, use getContext() or getClass() to retrieve the type");
    return 0;
}

const FrBClass* FrBUnresolvedTypeExpr::getClass() const
{
    frb_assert2(_type, "_type is a null pointer"
                     " - FrBUnresolvedTypeExpr::resolveAndCheck() probably not called");
    return _type;
}

std::ostream& FrBUnresolvedTypeExpr::put(std::ostream& stream) const
{
    if(_context)
        stream << *_context << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_MEMBER);
        
    if(_type)
        return stream << _type->fullName();
    else
        return stream << "ur_type_" << _name;
}

// /*     FrBUnresolvedIdExpr      */
// 
// 
// FrBUnresolvedIdExpr::FrBUnresolvedIdExpr(const String& name)
//  : _name(name), _type(0)
// {
// }
// 
// FrBUnresolvedIdExpr::~FrBUnresolvedIdExpr()
// {
// }
// 
// void FrBUnresolvedIdExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
// {
//     //
//     frb_assert(false);
// }
// 
// FrBBaseObject* FrBUnresolvedIdExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
// {
//     frb_assert2(_type, "_type is a null pointer"
//                        " - FrBUnresolvedIdExpr::resolveAndCheck() probably not called");
//     return 0;
// }
// 
// const FrBClass* FrBUnresolvedIdExpr::getClass() const
// {
//     frb_assert2(_type, "_type is a null pointer"
//                      " - FrBUnresolvedIdExpr::resolveAndCheck() probably not called");
//     return _type;
// }
// 
// std::ostream& FrBUnresolvedIdExpr::put(std::ostream& stream) const
// {
//     if(_type)
//         return stream << _type->name();
//     else
//         return stream << "ur_id_" << _name;
// }




/*     FrBUnresolvedIdWithContextExpr      */

/*     FrBUnresolvedIdWithContextExpr::*Evalutor      */
        

FrBUnresolvedIdWithContextExpr::FieldEvaluator::FieldEvaluator(FrBField * f)
 : _fl(f)
 {
 }
 
FrBBaseObject* FrBUnresolvedIdWithContextExpr::FieldEvaluator::eval(FrBBaseObject * me,
    FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    return me->getField(_fl->index());
}

const FrBClass* FrBUnresolvedIdWithContextExpr::FieldEvaluator::getClass() const
{
    return _fl->type();
}

bool FrBUnresolvedIdWithContextExpr::FieldEvaluator::needMe() const
{
    return !_fl->shared();
}

bool FrBUnresolvedIdWithContextExpr::FieldEvaluator::isAssignable() const
{
    return true;
}

void FrBUnresolvedIdWithContextExpr::FieldEvaluator::refAssign(FrBExecutionEnvironment& e,
        FrBBaseObject* me, FrBBaseObject* val) const throw (FrBEvaluationException)
{
    me->addField(_fl->index(), val);
}

/////////////////

FrBUnresolvedIdWithContextExpr::FunctionEvaluator::FunctionEvaluator(FrBFunction * f)
 : _fn(f)
 {
 }


FrBBaseObject* FrBUnresolvedIdWithContextExpr::FunctionEvaluator::eval(FrBBaseObject * me,
    FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    return e.addGarbagedObject(new FrBFunctionWrapper(_fn));
}

const FrBClass* FrBUnresolvedIdWithContextExpr::FunctionEvaluator::getClass() const
{
    return FrBFunctionWrapper::getCppClass();
}

bool FrBUnresolvedIdWithContextExpr::FunctionEvaluator::needMe() const
{
    return !_fn->shared();
}

/////////////////

FrBUnresolvedIdWithContextExpr::ClassEvaluator::ClassEvaluator(FrBClass * f)
 : _cl(f)
 {
 }
 
FrBBaseObject* FrBUnresolvedIdWithContextExpr::ClassEvaluator::eval(FrBBaseObject * me,
    FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    return e.addGarbagedObject(new FrBClassWrapper(_cl));
}

const FrBClass* FrBUnresolvedIdWithContextExpr::ClassEvaluator::getClass() const
{
    //return FrBClassWrapper::getCppClass();
    return _cl;
}

bool FrBUnresolvedIdWithContextExpr::ClassEvaluator::needMe() const
{
    return false;
}

    
/////////////////


FrBUnresolvedIdWithContextExpr::FrBUnresolvedIdWithContextExpr(FrBExpr * context, const String& name)
 : _context(context), _name(name), _evaluator(0), _context_resolved(false)
{
    frb_assert(context);
}

FrBUnresolvedIdWithContextExpr::~FrBUnresolvedIdWithContextExpr()
{
    if(_evaluator)
        delete _evaluator;
        
    delete _context;
}

void FrBUnresolvedIdWithContextExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    if(!_context_resolved)
        _context->resolveAndCheck(e);

    //const_cast pas tres propre mais pour le moment c'est tres bien comme ca :)
    FrBClass * current_class = const_cast<FrBClass *>(_context->getClass());

    try
    { /* look for a field first */
        _evaluator = new FieldEvaluator(current_class->findField(_name));
    }
    catch(FrBFieldNotFoundException ex)
    {
        /* look for a function */
        
//         FrBClass::FnPairIt pit = current_class->findFunctions(_name);
//         
//         if(pit.second->second)
//             throw FrBFunctionAmbiguityException(_name);
//         else if(pit.first != current_class->functionList()->end())
//             _evaluator = new FunctionEvaluator( pit.first->second );
//         else /* look for a class */
            _evaluator = new ClassEvaluator(e.getClassFromName(_name, current_class));
    }
}

FrBBaseObject* FrBUnresolvedIdWithContextExpr::eval(FrBExecutionEnvironment& e) const
    throw (FrBEvaluationException)
{
    frb_assert(_evaluator);

    FrBBaseObject * me = 0;

    if(_evaluator->needMe())
        me = _context->eval(e);

    return _evaluator->eval(me, e);
}

const FrBClass* FrBUnresolvedIdWithContextExpr::getClass() const
{
    frb_assert(_evaluator);
    return _evaluator->getClass();
}

bool FrBUnresolvedIdWithContextExpr::isAssignable() const
{
    frb_assert(_evaluator);
    return _evaluator->isAssignable();
}

void FrBUnresolvedIdWithContextExpr::refAssign(FrBExecutionEnvironment& e, FrBBaseObject* v) const
    throw (FrBEvaluationException)
{
    frb_assert(_evaluator);

    FrBBaseObject * me = 0;

    if(_evaluator->needMe())
        me = _context->eval(e);

    _evaluator->refAssign(e, me, v);
}

std::ostream& FrBUnresolvedIdWithContextExpr::put(std::ostream& stream) const
{
    if(_evaluator)
        return stream << _name;
    else
        return stream << "urwc_id_" << _name;
}





/*     FrBMemberOpExpr      */
// /*
// FrBMemberOpExpr::FrBMemberOpExpr(FrBExpr* lhs, FrBUnresolvedIdExpr* rhs)
//  : _lhs(lhs), _rhs(rhs)
// {
//     frb_assert2(rhs && lhs, "frbexpr.cpp::FrBMemberOpExpr::FrBMemberOpExpr()");
// }
// 
// FrBMemberOpExpr::~FrBMemberOpExpr()
// {
//     delete _lhs;
//     delete _rhs;
// }
// 
// void FrBMemberOpExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
// {
//     _lhs->resolveAndCheck(e);
//     
//     const FrBClass* cc  = _lhs->getClass();
//     const String & name =  _rhs->name();
//     
//     (void)cc;
//     (void)name;
//     
//     //TODO cf FrBUnresolvedIdExpr::resolveAndCheck() ms avec un contexte de recherche
//     // il faudrait definir une fonction commune, static, exemple da,s FrBTypeExpr
//     
// 
//     /* _rhs est soit une inner classe soit un membre soit tout ce qui est function */
//     
// //     /*  function/sub */
// //     FrBClass::FnPairIt pit = cc->findFunctions(name);
// //     
// //     if(pit.second->second)
// //     {
// //         /* ambiguity */
// //     
// //         puts("ID AMBIGUITY -- /* function/sub */\n");
// //         exit(0);
// //         
// //         break;
// //     }
// //     else if(pit.first != cc->functionList()->end())
// //     {
// //         /* found */
// //         
// //         $<expr>$ = new FrBObjectIdExpr(new FrBFunctionWrapper( pit.first->second ));
// //         
// //         puts("ID FOUND -- /* 4. function/sub */\n");
// //         
// //         break;
// //     }
// 
//    // throw FrBMemberNotFoundException(cc, name);
//    
//    _resolved = false;
//     
// }
// 
// FrBBaseObject* FrBMemberOpExpr::eval(FrBExecutionEnvironment&) const throw (FrBEvaluationException)
// {
//     return 0;
// }
// 
// const FrBClass* FrBMemberOpExpr::getClass() const
// {
//     return 0;
// }
// 
// std::ostream& FrBMemberOpExpr::put(std::ostream& stream) const
// {
//     return stream << '(' << *_lhs
//         << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_MEMBER)
//         << _rhs->name() << ')';
// }*/



/*              FrBFunctionCallExpr                */

FrBFunctionCallExpr::FrBFunctionCallExpr(FrBExpr* lhs, FrBExprList* rhs)
 : _lhs(lhs), _rhs(rhs), _fn(0), _me(0)
{
    frb_assert2(rhs && lhs, "frbexpr.cpp::FrBFunctionCallExpr::FrBFunctionCallExpr()");
}

FrBFunctionCallExpr::~FrBFunctionCallExpr()
{frb_warning2(false, "FrBFunctionCallExpr _lhs...");
    delete _lhs;
frb_warning2(false, "FrBFunctionCallExpr FrBExprList...");
    for(FrBExprList::iterator it = _rhs->begin(); it != _rhs->end(); ++it)
    {
        (*it)->put(std::cerr);
        delete (*it);
    }
frb_warning2(false, "FrBFunctionCallExpr rhs...");
    delete _rhs;
frb_warning2(false, "FrBFunctionCallExpr done.");
}

void FrBFunctionCallExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    String dbg_name = "unknow"; //for debug

    
    for(FrBExprList::iterator it = _rhs->begin(); it != _rhs->end(); ++it)
        (*it)->resolveAndCheck(e);
        
    FrBUnresolvedIdWithContextExpr * mo = dynamic_cast<FrBUnresolvedIdWithContextExpr*>(_lhs);
    
    if(mo) 
    {          /* case of a real function call ? */
        try
        {
            dbg_name = mo->name();

            mo->context()->resolveAndCheck(e);
            mo->setContextResolved(); /* to be sure that context'll not be resolved again after */

            _fn = mo->context()->getClass()->findFunction(mo->name(), *_rhs);
            _me = mo->context();

            return;
        }
        catch(...)  /* no, this is not a real funtion call */
        {
        }
        
    }

    /* case of the overload of the () operator */
    //frb_assert2(false, "overload of ()/[] not yet implemented");

    //_lhs->resolveAndCheck(e);

    throw FrBFunctionNotFoundException(dbg_name);

}


FrBBaseObject* FrBFunctionCallExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    frb_assert2(_fn, "frbexpr.cpp::FrBFunctionCallExpr::val() - _fn is a null pointer");

    FrBBaseObjectList rval;
    rval.reserve(_rhs->size());

    /* eval arguments */
    for(FrBExprList::iterator it = _rhs->begin(); it != _rhs->end(); ++it)
        rval.push_back((*it)->eval(e));

    FrBBaseObject* me = 0;

    if(!_fn->shared()) /* non-shared fn, me must be provided */
    {
        frb_assert(_me);
        me = _me->eval(e);
    }

    return _fn->execute(e, me, rval);

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
 
/*        FrBInsideMeExpr                */

FrBInsideMeExpr::FrBInsideMeExpr(FrBCodeFunction* f)
 : _fn(f)
{
}

FrBInsideMeExpr::~FrBInsideMeExpr()
{
}

void FrBInsideMeExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
  _varid = _fn->localVarCount();
}


FrBBaseObject* FrBInsideMeExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    return e.stack().getTopValue(_varid);
}

const FrBClass* FrBInsideMeExpr::getClass() const
{
    return _fn->container();
}

std::ostream& FrBInsideMeExpr::put(std::ostream& stream) const
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
  return e.stack().top();
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
    delete _lhs;
    delete _rhs;
}

void FrBRefAssignExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{

  _rhs->resolveAndCheck(e);
  _lhs->resolveAndCheck(e);

  frb_assert2(_lhs->isAssignable(), "invalid lvalue");

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
    
    //e.memory()->delLink(_lhs->eval(e));
    //e.memory()->addLink(r);
    
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
        if(!_fn->shared()) throw FrBFunctionNotFoundException(_fn->name());
    }
    catch(FrBFunctionNotFoundException)
    {
        try
        {
            _fn = cr->findOperator(_op, args);
            if(!_fn->shared()) throw FrBFunctionNotFoundException(_fn->name());
        }
        catch(FrBFunctionNotFoundException)
        {
            try
            {
                args.clear();
                args.push_back(cr);
                _fn = cl->findOperator(_op, args);
                if(_fn->shared()) throw FrBFunctionNotFoundException(_fn->name());
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


