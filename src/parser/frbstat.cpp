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


#include "frbstat.h"
#include "frbexpr.h"
#include "frbclass.h"
#include "../common/assert.h"
#include "../common/misc.h"
#include "frbmemory.h"
#include "frbbuiltinclasses.h"
#include "frbkeywords.h"
#include "frbtemplatespecializationenvironment.h"

std::ostream& operator<<(std::ostream& s, const FrBStatement& stat)
{
    return stat.put(s);
}

/*                  FrBStatementBlock                  */

FrBStatementBlock::FrBStatementBlock()
{
    _stats = new FrBStatementlist();
}

FrBStatementBlock::~FrBStatementBlock()
{
    for(FrBStatementlist::iterator it = _stats->begin(); it != _stats->end(); ++it)
        delete (*it);

    delete _stats;
}

FrBStatementBlock * FrBStatementBlock::specializeTemplateBlock(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBStatementBlock * cpy) const
{
    copy_not_null(cpy);

    cpy->_stats = new FrBStatementlist();

    for(FrBStatementlist::const_iterator it = _stats->begin(); it != _stats->end(); ++it)
	cpy->addStat( (*it)->specializeTemplate(e) );

    return cpy;
}

bool FrBStatementBlock::allPathContainsAReturn() const
{
  for(FrBStatementlist::const_iterator it = _stats->begin(); it != _stats->end(); ++it)
    if((*it)->allPathContainsAReturn())
      return true;

  return false;
}



/*          FrBStatement               */

FrBStatement * FrBStatement::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBStatement * cpy) const
{
    frb_assert2(cpy, "can not specialize this stat");
    return cpy;
}

/*        FrBBlockStatement                     */

FrBBlockStatement::FrBBlockStatement()
{
}

bool FrBBlockStatement::allPathContainsAReturn() const
{
  for(FrBStatementlist::const_iterator it = _stats->begin(); it != _stats->end(); ++it)
    if((*it)->allPathContainsAReturn())
      return true;

  return false;
}
    
void FrBBlockStatement::resolveAndCheck(FrBResolveEnvironment& e)
  throw (FrBResolveException)
{
  for(FrBStatementlist::iterator it = _stats->begin(); it != _stats->end(); ++it)
    (*it)->resolveAndCheck(e);

}

void FrBBlockStatement::execute(FrBExecutionEnvironment& e) const
  throw (FrBExecutionException)
{
  for(FrBStatementlist::const_iterator it = _stats->begin(); it != _stats->end(); ++it)
    (*it)->execute(e);
}

FrBStatement * FrBBlockStatement::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBStatement * cpy) const
{

    FrBStatementBlock::specializeTemplateBlock(e, static_cast<FrBBlockStatement*>(copy_not_null(cpy)));

    return cpy;
}
   
std::ostream& FrBBlockStatement::put(std::ostream& stream, int indent) const
{
  String str_indent(indent, '\t');

  indent++;

  for(FrBStatementlist::const_iterator it = _stats->begin(); it != _stats->end(); ++it)
  {
    stream << std::endl << str_indent << '\t';
    (*it)->put(stream, indent);
  }

  return stream;
}
    


/*       FrBConditionalBlockStatement          */

bool FrBConditionalBlockStatement::allPathContainsAReturn() const
{
  return false;
}

void FrBConditionalBlockStatement::execute(FrBExecutionEnvironment& e) const
  throw (FrBExecutionException)
{
  (void)executeCond(e);
}

bool FrBConditionalBlockStatement::executeCond(FrBExecutionEnvironment& e) const
  throw (FrBExecutionException)
{
  bool cond = evalCond(e);

  if(cond)
    FrBBlockStatement::execute(e);

  return cond;
}



/*                    FrBElseIfStatement                  */



FrBElseIfStatement::FrBElseIfStatement(FrBExpr * cond)
  : _cond(cond)
{
  frb_assert(cond);
}

void  FrBElseIfStatement::resolveAndCheck(FrBResolveEnvironment& e)
  throw (FrBResolveException)
{
  _cond->resolveAndCheck(e);
  if(!_cond->getClass()->isCompatibleWith(FrBBool::getCppClass()))
    throw FrBIncompatibleClassException(_cond->getClass(), FrBBool::getCppClass());

  FrBBlockStatement::resolveAndCheck(e);

}

bool FrBElseIfStatement::evalCond(FrBExecutionEnvironment& e) const
  throw (FrBExecutionException)
{
  FrBBaseObject * o = FrBClass::forceConvert(_cond->eval(e), FrBBool::getCppClass());
  return (static_cast<FrBBool*>(o))->value();
}

FrBStatement * FrBElseIfStatement::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBStatement * cpy) const
{
    FrBElseIfStatement* ret = static_cast<FrBElseIfStatement*>(copy_not_null(cpy));

    FrBStatementBlock::specializeTemplateBlock(e, ret);
    ret->_cond = _cond->specializeTemplate(e);

    return ret;
}


std::ostream& FrBElseIfStatement::put(std::ostream& stream, int indent) const
{
  stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_IF) << ' '
	 << *_cond << ' '
	 << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_THEN);
  //	 << std::endl << String(indent, '\t');

  return FrBBlockStatement::put(stream, indent);
}
    
FrBElseIfStatement::~FrBElseIfStatement()
{
    delete_expr(_cond);
}

/*         FrBElseStatement            */

bool FrBElseStatement::evalCond(FrBExecutionEnvironment& e) const throw (FrBExecutionException)
{
  return true;
}

FrBStatement * FrBElseStatement::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBStatement * cpy) const
{
    copy_not_null(cpy);

    FrBStatementBlock::specializeTemplateBlock(e, static_cast<FrBElseStatement*>(cpy));

    return cpy;
}

/*       FrBIfStatement                   */

FrBIfStatement::FrBIfStatement()
  : _has_else(false)
{
    _conds = new FrBCondList();
}

bool FrBIfStatement::allPathContainsAReturn() const
{
  if(!_has_else)
    return false;

  for(FrBCondList::const_iterator it = _conds->begin(); it != _conds->end(); ++it)
    if(! (*it)->FrBBlockStatement::allPathContainsAReturn())
      return false;

  return true;
}

    
void  FrBIfStatement::resolveAndCheck(FrBResolveEnvironment& e)
  throw (FrBResolveException)
{
  for(FrBCondList::iterator it = _conds->begin(); it != _conds->end(); ++it)
    (*it)->resolveAndCheck(e);
}

void  FrBIfStatement::execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException)
{
  FrBCondList::const_iterator it = _conds->begin();
  while( it != _conds->end() && (!((*it)->executeCond(e))) )
    ++it;
}


FrBStatement * FrBIfStatement::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBStatement * cpy) const
{
    FrBIfStatement * c = static_cast<FrBIfStatement *>(copy_not_null(cpy));

    c->_conds = new FrBCondList();

    for(FrBCondList::const_iterator it = _conds->begin(); it != _conds->end(); ++it)
	c->addCond( static_cast<FrBConditionalBlockStatement*>( (*it)->specializeTemplate(e) ) );

    return c;
}

std::ostream&  FrBIfStatement::put(std::ostream& stream, int indent) const
{
  String str_indent(indent, '\t');

  FrBCondList::const_iterator it = _conds->begin();


  if(it != _conds->end())
  {
    (*it)->put(stream, indent);
    ++it;
  }

  while(it != _conds->end())
  {
    stream << std::endl << str_indent
	   << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_ELSE)
	   << ' ';
    (*it)->put(stream, indent);
    ++it;
  }

  return stream << std::endl << str_indent
		<< FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_END)
		<< ' '
		<< FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_IF);
}
    
FrBIfStatement::~FrBIfStatement()
{
    for(FrBCondList::iterator it = _conds->begin(); it != _conds->end(); ++it)
        delete (*it);

    delete _conds;
}







/*                 FrBDeclareStatement              */

FrBDeclareStatement::FrBDeclareStatement(FrBCodeFunction * f, int nb,
					    FrBTypeExpr * t, FrBExpr * init_val)
    : _fn(f), _type(t), _init(init_val)
{
    frb_assert(t && nb > 0);
    _varsid = new VarIDVector();
    _varsid->reserve(nb);
}

void FrBDeclareStatement::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    for(VarIDVector::iterator it = _varsid->begin(); it != _varsid->end(); ++it)
        *it += _fn->localVarCount();

    _type->resolveAndCheck(e);
    
    if(_init)
    {
        _init->resolveAndCheck(e);
        if(!_init->getClass()->isCompatibleWith(_type->getClass()))
            throw FrBIncompatibleClassException(_init->getClass(), _type->getClass());
    }
    else
    {
	/* check for default ctor */
	if(!_type->getClass()->hasDefaultCtor())
	    throw FrBDecWithoutDefaultCtorException(_type->getClass());
    }
}

void FrBDeclareStatement::execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException)
{

    for(VarIDVector::const_iterator it = _varsid->begin(); it != _varsid->end(); ++it)
    {
	FrBBaseObject * init_val = ((_init == 0) ? _type->getClass()->createInstance(e) :
                                  FrBClass::forceConvert(_init->eval(e), _type->getClass()));

        e.stack().setTopValue(*it, init_val);
    }
}

FrBStatement * FrBDeclareStatement::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBStatement * cpy) const
{
    copy_not_null(cpy);

    FrBDeclareStatement * c = static_cast<FrBDeclareStatement *>(cpy);

    int n = _varsid->size();
    c->_varsid = new VarIDVector(n);

    for(int i = 0; i < n; ++i)
	(*c->_varsid)[i] = (*_varsid)[i];

    frb_assert(Misc::isKindOf<FrBCodeFunction>(e.currentMember()));

    c->_fn = static_cast<FrBCodeFunction*>(e.currentMember());


    c->_type = static_cast<FrBTypeExpr*>(_type->specializeTemplate(e));

    if(c->_init)
	c->_init = _init->specializeTemplate(e);

    return cpy;
}

std::ostream& FrBDeclareStatement::put(std::ostream& stream, int) const
{
    stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_DECLARE);

    VarIDVector::const_iterator it = _varsid->begin();

    if(it != _varsid->end())
    {
           stream << " local_var_" << *it;
            ++it;
    }

    while(it != _varsid->end())
    {
        stream << ", local_var_" << *it;
        ++it;
    }

    stream  << ' ' << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_AS) << ' '
            << *_type;

    if(_init)
      stream << ' '
	     << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_ASSIGN_REF)
	     << ' ' << *_init;
    
    return stream;
}

FrBDeclareStatement::~FrBDeclareStatement()
{
    delete_expr(_init);

    delete_expr(_type);

    delete _varsid;
}



/*                 FrBExprStatement                */
FrBExprStatement::FrBExprStatement(FrBExpr* expr)
    : _expr(expr)
{
    frb_assert2(expr, "frbparsingtree.h::FrBExprStatement::FrBExprStatement()");
}


void FrBExprStatement::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    _expr->resolveAndCheck(e);
}

void FrBExprStatement::execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException)
{
    _expr->eval(e);
}


FrBStatement * FrBExprStatement::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBStatement * cpy) const
{
    copy_not_null(cpy);

    static_cast<FrBExprStatement*>(cpy)->_expr = _expr->specializeTemplate(e);

    return cpy;
}

std::ostream& FrBExprStatement::put(std::ostream& stream, int) const
{
  return stream << *_expr;
}

FrBExprStatement::~FrBExprStatement()
{
    delete_expr(_expr);
}




/*        FrBReturnStatement        */

FrBReturnStatement::FrBReturnStatement(const FrBFunction * f, FrBExpr* v)
  : _val(v), _fn(f)
{
  frb_assert(f);
}

bool FrBReturnStatement::allPathContainsAReturn() const
{
  return true;
}
    
void FrBReturnStatement::resolveAndCheck(FrBResolveEnvironment& e)
  throw (FrBResolveException)
{
  if(_val)
  {
    _val->resolveAndCheck(e);
    if(!_val->getClass()->isCompatibleWith(_fn->returnType()))
      throw FrBIncompatibleClassException(_val->getClass(), _fn->returnType());
  }
}

void FrBReturnStatement::execute(FrBExecutionEnvironment& e) const
  throw (FrBExecutionException)
{
  throw FrBReturnException( _val ? 
 		    FrBClass::forceConvert(_val->eval(e), _fn->returnType()) : 0 );
}

std::ostream& FrBReturnStatement::put(std::ostream& stream, int) const
{
  stream << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_RETURN);

  if(_val)
    stream << ' ' << *_val;

  return stream;
}

FrBStatement * FrBReturnStatement::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBStatement * cpy) const
{
    copy_not_null(cpy);

    FrBReturnStatement * c = static_cast<FrBReturnStatement*>(cpy);

    c->_val = _val->specializeTemplate(e);
    
    frb_assert(Misc::isKindOf<FrBFunction>(e.currentMember()));
    c->_fn = static_cast<FrBFunction*>(e.currentMember());

    return cpy;
}
    
FrBReturnStatement::~FrBReturnStatement()
{
  delete_expr(_val);
}


/*             FrBForLoopStatement                    */

FrBForLoopStatement::FrBForLoopStatement(FrBExpr * var, FrBExpr * init, int direction, FrBExpr * max, FrBExpr * step)
{
    if(step) /* var += step / var -= step */
	_incrementor = new FrBBinOpExpr(var, step, (direction == 1) ? FrBKeywords::FRB_KW_OP_ADD_ASSIGN : FrBKeywords::FRB_KW_OP_SUB_ASSIGN);
    else /* var++ / var-- */
	_incrementor = new FrBUnaryOpExpr(var, (direction == 1) ? FrBKeywords::FRB_KW_OP_INCR : FrBKeywords::FRB_KW_OP_DECR);

    /* var := init */
    _assignator = new FrBRefAssignExpr(var, init);

    /* var <= max / var >= max */
    _bounds_checker = new FrBBinOpExpr(var, max, (direction == 1) ? FrBKeywords::FRB_KW_OP_LE : FrBKeywords::FRB_KW_OP_GE);

    /* var is used in 3 differents exprs, so we add 2 refs */
    var->addRef(2);
}


void FrBForLoopStatement::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{

    _incrementor->resolveAndCheck(e);

    _assignator->rhs()->resolveAndCheck(e);
    _assignator->partialResolveAndCheck(e);

    _bounds_checker->rhs()->resolveAndCheck(e);
    _bounds_checker->partialResolveAndCheck(e);

    if(!_bounds_checker->getClass()->isCompatibleWith(FrBBool::getCppClass()))
	throw FrBIncompatibleClassException(_bounds_checker->getClass(), FrBBool::getCppClass());

    FrBBlockStatement::resolveAndCheck(e);
}


void FrBForLoopStatement::execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException)
{
   //TODO Faire break & continue

    /* var := init */
    _assignator->eval(e);

    /* while(var <= max) */
    //while( (static_cast<FrBBool*>(FrBClass::forceConvert(_bounds_checker->eval(e), FrBBool::getCppClass())))->value() )
    //while( _bounds_checker->eval(e) == FrBBool::trueValue()  )
    while( ((FrBBool*)(_bounds_checker->eval(e)))->value() )
    {
	FrBBlockStatement::execute(e);

	_incrementor->eval(e);
    }

}

FrBStatement * FrBForLoopStatement::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBStatement * cpy) const
{
    copy_not_null(cpy);

    FrBForLoopStatement * c = static_cast<FrBForLoopStatement*>(cpy);

    c->_incrementor = _incrementor->specializeTemplate(e);
    c->_bounds_checker = static_cast<FrBBinOpBaseExpr*>(_bounds_checker->specializeTemplate(e));
    c->_assignator = static_cast<FrBBinOpBaseExpr*>(_assignator->specializeTemplate(e));

    return cpy;
}

std::ostream& FrBForLoopStatement::put(std::ostream& stream, int indent) const
{
    String str_indent(indent, '\t');

    _assignator->put(stream);

    stream << std::endl << str_indent << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_WHILE) << ' ';
    _bounds_checker->put(stream);

    FrBBlockStatement::put(stream, indent);
    stream << std::endl << str_indent << '\t';
    _incrementor->put(stream);

    stream << std::endl << str_indent << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_END) << ' '
	   << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_WHILE);

    return stream;
}

bool FrBForLoopStatement::allPathContainsAReturn() const
{
    return false;
}

FrBForLoopStatement::~FrBForLoopStatement()
{

    delete_expr(_incrementor);
    delete_expr(_assignator);
    delete_expr(_bounds_checker);

}



