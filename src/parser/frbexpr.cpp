/***************************************************************************
 *   This file is part of the FrB project                                  *
 *                                                                         *
 *   FrB project                                                           *
 *                                                                         *
 *   Copyright (C) 2006, 2007 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://frb.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/


#include "frbexpr.h"
#include "../common/assert.h"
#include "../common/misc.h"
#include "frbkeywords.h"
#include "frbfunction.h"
#include "frbresolveenvironment.h"
#include "frbbuiltinclasses.h"

/*          FrBExpr          */

std::ostream& operator<<(std::ostream& s, const FrBExpr& expr)
{
    return expr.put(s);
}


const FrBClass* FrBExpr::getRealClass() const
{
    return getClass();
}

bool FrBExpr::isAssignable() const
{
    return false;
}

bool FrBExpr::isInstance() const
{
    return true;
}

void FrBExpr::refAssign(FrBExecutionEnvironment&, FrBBaseObject* o) const throw (FrBEvaluationException)
{
    //frb_assert(isAssignable());
    frb_assert(o);

    throw FrBInvalidLValueException(this);
}


void delete_expr(FrBExpr* e)
{
    if(!e) return;

    e->delRef();
    
    if(e->deletable())
        delete e;
}

/*         FrBLocalVarExpr        */

FrBLocalVarExpr::FrBLocalVarExpr(FrBCodeFunction * fn, FrBTypeExpr * t, int varid)
 : _fn(fn), _type(t), _varid(varid)
{
}

FrBLocalVarExpr::~FrBLocalVarExpr()
{
    //delete _type; //done in functioncode destructor
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
        
        delete_expr(_context); /* we don't need _context anymore */
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


/*         FrBTemplateTypeExpr            */


FrBTemplateTypeExpr::FrBTemplateTypeExpr(FrBClass::template_count_t p,  FrBCodeClass * c)
    : _currentClass(c), _p(p)
{
    frb_assert(c);
}

FrBTemplateTypeExpr::~FrBTemplateTypeExpr()
{
}
    
void FrBTemplateTypeExpr::resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException)
{
}

FrBBaseObject* FrBTemplateTypeExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    return e.addGarbagedObject(new FrBClassWrapper(getClass()));
}

const FrBClass* FrBTemplateTypeExpr::getClass() const
{
    frb_assert2(false, "uninstancied template");
    return 0;
}


bool FrBTemplateTypeExpr::isAssignable() const
{
    return false;
}

bool FrBTemplateTypeExpr::isInstance() const
{
    return false;
}


std::ostream& FrBTemplateTypeExpr::put(std::ostream& stream) const
{
    return stream << "template_parameter_" << _p;
}


/*     FrBUnresolvedIdWithContextExpr      */

/*     FrBUnresolvedIdWithContextExpr::*Evalutor      */
        

FrBUnresolvedIdWithContextExpr::FieldEvaluator::FieldEvaluator(FrBField * f)
 : _fl(f)
 {
 }
 
FrBBaseObject* FrBUnresolvedIdWithContextExpr::FieldEvaluator::eval(FrBBaseObject * me,
    FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    if(_fl->shared()) /* shared field */
	return e.sharedMem().getSharedField(_fl);
    else /* non shared field */
	return me->getField(_fl);
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
    if(_fl->shared()) /* shared field */
	e.sharedMem().setSharedField(_fl, val);
    else /* non shared field */
	me->addField(_fl, val);


}

bool FrBUnresolvedIdWithContextExpr::FieldEvaluator::isInstance() const
{
    return true;
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

bool FrBUnresolvedIdWithContextExpr::FunctionEvaluator::isInstance() const
{
    return true;
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
    return FrBClassWrapper::getCppClass();
}

const FrBClass* FrBUnresolvedIdWithContextExpr::ClassEvaluator::getRealClass() const
{
    return _cl;
}

bool FrBUnresolvedIdWithContextExpr::ClassEvaluator::needMe() const
{
    return false;
}

bool FrBUnresolvedIdWithContextExpr::ClassEvaluator::isInstance() const
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
    delete _evaluator;
    delete_expr(_context);
}

void FrBUnresolvedIdWithContextExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    frb_assert(_context);

    if(!_context_resolved)
        _context->resolveAndCheck(e);

    //const_cast pas tres propre mais pour le moment c'est tres bien comme ca :)
    FrBClass * current_class = const_cast<FrBClass *>(_context->getRealClass());

    try
    { /* is this a field ? */
	FrBField * f = current_class->findField(_name);

	/* yes. is the context an instance or a class ? */
	if(!_context->isInstance())
	{
	    /* this is a class, so field must be shared */
	    if(!f->shared())
		throw FrBInvalidNonSharedException(f);
	}

	/* ok fine, we'll behave like a field now */
	_evaluator = new FieldEvaluator(f);
    }
    catch(FrBFieldNotFoundException ex)
    {

	try /* try for a class */
	{

	     /* no, is this a class ? */
	     FrBClass * c = e.getClassFromName(_name, current_class);

	     /* ok fine, we'll behave like a class now */
	     _evaluator = new ClassEvaluator(c);

	}
	catch(FrBClassNotFoundException ex2)
	{ /* this is not a class, try for a function*/

	    FrBClass::FnPairIt pit = current_class->findFunctions(_name); //is this a function ?

	    if(pit.first == current_class->functionList()->end())
		/* nop */
		throw ex2; 
	    else if(pit.first == --pit.second)
		/* yes it is */
		_evaluator = new FunctionEvaluator( pit.first->second );
	    else
		/* damned, it matches several functions */
		throw FrBFunctionAmbiguityException(_name);

	}

    }

    /* if _context is not an instance, it is not more useful */
    if(!_context->isInstance())
    {
	delete_expr(_context);
	_context = 0;
    }

}

FrBBaseObject* FrBUnresolvedIdWithContextExpr::eval(FrBExecutionEnvironment& e) const
    throw (FrBEvaluationException)
{
    frb_assert(_evaluator);

    FrBBaseObject * me = FrBNull::nullValue();

    if(_context)
	me = _context->eval(e);

    if( !Misc::isKindOf<FunctionEvaluator>(_evaluator) && _evaluator->needMe() && FrBNull::isNull(me))
	    throw FrBNullReferenceException();

    //if(isInstance())
	return _evaluator->eval(me, e);
//    else
	//return  FrBNull::nullValue();
}

const FrBClass* FrBUnresolvedIdWithContextExpr::getClass() const
{
    frb_assert(_evaluator);
    return _evaluator->getClass();
}

const FrBClass* FrBUnresolvedIdWithContextExpr::getRealClass() const
{
    frb_assert(_evaluator);
    return _evaluator->getRealClass();
}

bool FrBUnresolvedIdWithContextExpr::isAssignable() const
{
    frb_assert(_evaluator);
    return _evaluator->isAssignable();
}

bool FrBUnresolvedIdWithContextExpr::isInstance() const
{
   frb_assert(_evaluator);
   return _evaluator->isInstance();
}


void FrBUnresolvedIdWithContextExpr::refAssign(FrBExecutionEnvironment& e, FrBBaseObject* v) const
    throw (FrBEvaluationException)
{
    frb_assert(_evaluator);

    FrBBaseObject * me = FrBNull::nullValue();

    if(_context)
	me = _context->eval(e);

    if(_evaluator->needMe() && FrBNull::isNull(me))
	    throw FrBNullReferenceException();

    frb_assert(isInstance());

    _evaluator->refAssign(e, me, v);
}

std::ostream& FrBUnresolvedIdWithContextExpr::put(std::ostream& stream) const
{
    if(_evaluator)
        return stream << _evaluator->name();
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
{
    delete_expr(_lhs);

    for(FrBExprList::iterator it = _rhs->begin(); it != _rhs->end(); ++it)
        delete_expr(*it);

    _rhs->clear();

    delete _rhs;

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

            _fn = mo->context()->getRealClass()->findFunction(mo->name(), *_rhs);
            _me = mo->context();


        }
        catch(FrBFunctionNotFoundException ex)  /* no, this is not a real funtion call */
        {
	    /* case of the overload of the () operator */

	    _lhs->resolveAndCheck(e);
            _fn = _lhs->getClass()->findOperator(FrBKeywords::FRB_KW_OP_O_BRACKET, *_rhs);
            _me = _lhs;
	 
        }

	/* yes. is the context an instance or a class ? */
	if(!_me->isInstance())
	{
	    /* this is a class, so fn must be shared */
	    if(!_fn->shared())
		throw FrBInvalidNonSharedException(_fn);
	}

	return;

    }

    /* case of the overload of the () operator */
    
    _lhs->resolveAndCheck(e);
    _fn = _lhs->getClass()->findOperator(FrBKeywords::FRB_KW_OP_O_BRACKET, *_rhs);
    _me = _lhs;


}


FrBBaseObject* FrBFunctionCallExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    frb_assert2(_fn, "frbexpr.cpp::FrBFunctionCallExpr::val() - _fn is a null pointer");

    FrBBaseObjectList rval;
    rval.reserve(_rhs->size());

    /* eval arguments */
    for(FrBExprList::iterator it = _rhs->begin(); it != _rhs->end(); ++it)
        rval.push_back((*it)->eval(e));

    FrBBaseObject* me = FrBNull::nullValue();

    if(!_fn->shared()) /* non-shared fn, me must be provided */
    {
        frb_assert(_me);
        me = _me->eval(e);

	if(!me || FrBNull::isNull(me))
	    //throw FrBInvalidNonSharedException(_fn);
	    throw FrBNullReferenceException();
    }

    if(_fn->event())
    {
	e.raiseEvent(me, _fn, rval);
	return FrBNull::nullValue();
    }
    else
	return _fn->execute(e, me, rval);

}

const FrBClass* FrBFunctionCallExpr::getClass() const
{
    frb_assert(_fn);
    return _fn->returnType();
}

std::ostream& FrBFunctionCallExpr::put(std::ostream& stream) const
{
    if(_fn)
    {
	FrBUnresolvedIdWithContextExpr * mo = dynamic_cast<FrBUnresolvedIdWithContextExpr*>(_lhs);
    
	if(mo) 
	    stream <<  *(mo->context()) << '.' << _fn->name();
	else
	    stream << *_lhs;
	
    }
    else
	stream << *_lhs;


    stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_O_BRACKET);

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

       
/*          FrBMeExpr                 */

FrBMeExpr::FrBMeExpr(bool nonSharedContext)
    : _nonSharedContext(nonSharedContext)
{
}


bool FrBMeExpr::isInstance() const
{
    return _nonSharedContext;
}


std::ostream& FrBMeExpr::put(std::ostream& stream) const
{
    return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_ME) 
		  << "_" << (_nonSharedContext ? "non_shared" : "shared");
}
 
/*        FrBInsideMeExpr                */

FrBInsideMeExpr::FrBInsideMeExpr(FrBCodeFunction* f, bool nonSharedContext)
 : FrBMeExpr(nonSharedContext), _fn(f)
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




/*              FrBOutisdeMeExpr           */

FrBOutsideMeExpr::FrBOutsideMeExpr(FrBField * t, bool nonSharedContext)
  : FrBMeExpr(nonSharedContext),_field(t)
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
  return _field->container();
}

/*           FrBBinOpBaseExpr            */


FrBBinOpBaseExpr::FrBBinOpBaseExpr(FrBExpr* lhs, FrBExpr* rhs)
    : _lhs(lhs), _rhs(rhs)
{
    frb_assert(lhs && rhs);
}


FrBBinOpBaseExpr::~FrBBinOpBaseExpr()
{
    delete_expr(_rhs);
    delete_expr(_lhs);
}


void FrBBinOpBaseExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    _lhs->resolveAndCheck(e);
    _rhs->resolveAndCheck(e);
    partialResolveAndCheck(e);
}



/*           FrBRefAssignExpr                    */


FrBRefAssignExpr::FrBRefAssignExpr(FrBExpr* lhs, FrBExpr* rhs)
 : FrBBinOpBaseExpr(lhs, rhs)
{
}


void FrBRefAssignExpr::partialResolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
  frb_assert2(_lhs->isAssignable(), "invalid lvalue");

  if(!_rhs->getClass()->isCompatibleWith(_lhs->getClass()))
    throw FrBIncompatibleClassException(_rhs->getClass(), _lhs->getClass());
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
    : FrBBinOpBaseExpr(lhs, rhs), _op(op), _fn(0)
{
}


void FrBBinOpExpr::partialResolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    
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
		    String(FrBKeywords::getKeyword(FrBKeywords::FRB_KW_OPERATOR)).append(" ")
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
        return _fn->execute(e, FrBNull::nullValue(), lval, rval);
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
      return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_O_BRACKET) << *_lhs << ' '
		    << FrBKeywords::getKeywordOrSymbol(_op) << ' '
		    << *_rhs << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_C_BRACKET);
    else
        return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_O_BRACKET)
		      << *_lhs << " <ur_or_def:" << FrBKeywords::getKeywordOrSymbol(_op)
		      << "> " << *_rhs << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_C_BRACKET);
}


/*         FrBIsOpExpr              */

FrBIsOpExpr::FrBIsOpExpr(FrBExpr* lhs, FrBExpr* rhs)
    : FrBBinOpBaseExpr(lhs, rhs)
{
}
    
void FrBIsOpExpr::partialResolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException)
{
}

FrBBaseObject* FrBIsOpExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    return FrBBool::fromCPPBool( _lhs->eval(e) ==  _rhs->eval(e) );
}

const FrBClass* FrBIsOpExpr::getClass() const
{
    return FrBBool::getCppClass();
}

std::ostream& FrBIsOpExpr::put(std::ostream& stream) const
{
      return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_O_BRACKET) << *_lhs << ' '
		    << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_IS) << ' '
		    << *_rhs << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_C_BRACKET);
}



/*                     FrBLogOpBaseExpr                   */
    

FrBLogOpBaseExpr::FrBLogOpBaseExpr(FrBExpr* lhs, FrBExpr* rhs, int op)
    : FrBBinOpExpr(lhs, rhs, op)
{
}
    
void FrBLogOpBaseExpr::partialResolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    // That's not the 'operator(Bool, Bool)', try to get the wanted overload
    if( ! (_rhs->getClass() == FrBBool::getCppClass() && _lhs->getClass() == FrBBool::getCppClass()) )
	FrBBinOpExpr::partialResolveAndCheck(e);
}

const FrBClass* FrBLogOpBaseExpr::getClass() const
{
    if(_fn)
	return _fn->returnType();
    else
	return FrBBool::getCppClass();
}


/*                     FrBLogAndOpExpr                   */
    

FrBLogAndOpExpr::FrBLogAndOpExpr(FrBExpr* lhs, FrBExpr* rhs)
    : FrBLogOpBaseExpr(lhs, rhs, FrBKeywords::FRB_KW_OP_LOG_AND)
{
}


FrBBaseObject* FrBLogAndOpExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    frb_assert(_lhs);
    frb_assert(_rhs);

    if(_fn) /* execute overload */
	return FrBBinOpExpr::eval(e);
    else /* execute standard And, with lazy eval */
	return FrBBool::fromCPPBool( (static_cast<FrBBool*>(_lhs->eval(e))->value()) && (static_cast<FrBBool*>(_rhs->eval(e))->value()) );
}

/*                     FrBLogOrOpExpr                   */
    

FrBLogOrOpExpr::FrBLogOrOpExpr(FrBExpr* lhs, FrBExpr* rhs)
    : FrBLogOpBaseExpr(lhs, rhs, FrBKeywords::FRB_KW_OP_LOG_OR)
{
}


FrBBaseObject* FrBLogOrOpExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    frb_assert(_lhs);
    frb_assert(_rhs);

    if(_fn) /* execute overload */
	return FrBBinOpExpr::eval(e);
    else /* execute standard And, with lazy eval */
	return FrBBool::fromCPPBool( (static_cast<FrBBool*>(_lhs->eval(e))->value()) || (static_cast<FrBBool*>(_rhs->eval(e))->value()) );
}



/*           FrBUnaryOpExpr         */
//  FrBExpr     *_e;
//  int          _op;
//  FrBFunction *_fn;
    
FrBUnaryOpExpr::FrBUnaryOpExpr(FrBExpr* e, int op)
    : _e(e), _op(op), _fn(0)
{
    frb_assert(e);
}

FrBUnaryOpExpr::~FrBUnaryOpExpr()
{
    delete_expr(_e);
}
    
void FrBUnaryOpExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    _e->resolveAndCheck(e);
   
    const FrBClass * ce = _e->getClass();
    
    FrBConstClassList args;
    args.push_back(ce);
    
    try
    {
        _fn = ce->findOperator(_op, args);
        if(!_fn->shared()) throw FrBFunctionNotFoundException(_fn->name());
    }
    catch(FrBFunctionNotFoundException)
    {
	try
	{
	    args.clear();
	    _fn = ce->findOperator(_op, args);
	    if(_fn->shared()) throw FrBFunctionNotFoundException(_fn->name());
	}
	catch(FrBFunctionNotFoundException)
	{
	    FrBFunctionNotFoundException ex(
		String(FrBKeywords::getKeyword(FrBKeywords::FRB_KW_OPERATOR)).append(" ")
		.append(FrBKeywords::getKeywordOrSymbol(_op)),
		args
		);
                
	    const FrBClass::OperatorContainer * ops = ce->operatorList();
                
	    for(FrBClass::OperatorContainer::const_iterator itf = ops->begin(); itf != ops->end(); ++itf)
		ex.addCandidate(itf->second);

	    throw ex;
	}
     
    }
}

FrBBaseObject* FrBUnaryOpExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    frb_assert(_fn);
        
    FrBBaseObject* val = _e->eval(e);
    
    if(_fn->shared())
        return _fn->execute(e, FrBNull::nullValue(), val);
    else
        return _fn->execute(e, val);
        


}

const FrBClass* FrBUnaryOpExpr::getClass() const
{
    frb_assert(_fn);
    return _fn->returnType();
}

std::ostream& FrBUnaryOpExpr::put(std::ostream& stream) const
{
    if(_fn)
      return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_O_BRACKET)
		    << FrBKeywords::getKeywordOrSymbol(_op) << ' ' << *_e
		    << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_C_BRACKET);
    else
      return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_O_BRACKET)
		    << " <unresolved:" << FrBKeywords::getKeywordOrSymbol(_op) << "> " << *_e
		    << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_C_BRACKET);
}


/*            FrBCastExpr            */

FrBCastExpr::FrBCastExpr(FrBExpr* type, FrBExpr* val)
    : _type(type), _val(val)
{
}

FrBCastExpr::~FrBCastExpr()
{
    delete_expr(_type);
    delete_expr(_val);
}
    
void FrBCastExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    _type->resolveAndCheck(e);
    _val->resolveAndCheck(e);

    if(_type->getClass() != FrBClassWrapper::getCppClass())
	throw FrBIncompatibleClassException(_type->getClass(), FrBClassWrapper::getCppClass());


}

FrBBaseObject* FrBCastExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    FrBBaseObject * o = _val->eval(e);
    //FrBClassWrapper * true_type =  static_cast<FrBClassWrapper*>(_type->eval(e));

    if(!o->getClass()->isCompatibleWith(_val->getClass()))
	throw FrBIncompatibleClassException(o->getClass(), _val->getClass());

    return o;
}

const FrBClass* FrBCastExpr::getClass() const
{
    return _type->getRealClass();
}

std::ostream& FrBCastExpr::put(std::ostream& stream) const
{
    return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_CAST)
		  << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_LT) << *_type
		  << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_GT) 
		  << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_O_BRACKET) << *_val 
		  << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_C_BRACKET);
}



/*              FrBNewExpr             */


FrBNewExpr::FrBNewExpr(FrBTypeExpr *type, FrBExprList * args)
    : _type(type), _args(args), _ctor(0)
{
}

FrBNewExpr::~FrBNewExpr()
{
    delete_expr(_type);

    for(FrBExprList::iterator it = _args->begin(); it != _args->end(); ++it)
        delete_expr(*it);

    _args->clear();

    delete _args;
}

void  FrBNewExpr::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    _type->resolveAndCheck(e);

    for(FrBExprList::iterator it = _args->begin(); it != _args->end(); ++it)
        (*it)->resolveAndCheck(e);


    _ctor = _type->getClass()->findConstructor(*_args);
        
}

const FrBClass*  FrBNewExpr::getClass() const
{
    frb_assert(_ctor);
    return _type->getClass();
}

FrBBaseObject*  FrBNewExpr::eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
{
    FrBBaseObjectList rval;
    rval.reserve(_args->size());

    /* eval constructor arguments */
    for(FrBExprList::iterator it = _args->begin(); it != _args->end(); ++it)
        rval.push_back((*it)->eval(e));

    return _type->getClass()->createInstance(e, _ctor, rval);
}


std::ostream&  FrBNewExpr::put(std::ostream& stream) const
{
    stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_NEW) << ' ';


    if(_ctor)
	stream << _type->getClass()->fullName();
    else
	stream << *_type;

    stream  << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_O_BRACKET);

    FrBExprList::const_iterator it = _args->begin();

    if( it != _args->end() )
    {
      stream << **it;
      ++it;
    }

    while(it != _args->end())
    {
      stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_LIST_SEP) 
	     << ' ' << **it;
      ++it;
    }
        
            
    return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_C_BRACKET);
}



/*          FrBLiteralExpr            */



std::ostream& FrBIntExpr::put(std::ostream& stream) const
{
  return stream << value();
}

std::ostream& FrBBaseBoolExpr::put(std::ostream& stream) const
{
  if(value())
    return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_TRUE);
  else
    return stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_FALSE);
}

std::ostream& FrBStringExpr::put(std::ostream& stream) const
{
  return stream << '\"' << value() << '\"';
}



/*         FrBNullExpr                */

//FrBNullExpr * FrBNullExpr::_null_expr = 0;
    
FrBNullExpr::FrBNullExpr()
{
}


FrBNullExpr::~FrBNullExpr()
{
}

/*FrBNullExpr * FrBNullExpr::nullExpr()
{
    if(!_null_expr)
        _null_expr = new FrBNullExpr();
        
    return _null_expr;
}*/

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

