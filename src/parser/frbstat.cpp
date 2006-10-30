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


#include "frbstat.h"
#include "frbexpr.h"
#include "frbclass.h"
#include "../common/assert.h"
#include "frbmemory.h"
#include "frbbuiltinclasses.h"


std::ostream& operator<<(std::ostream& s, const FrBStatement& stat)
{
    return stat.put(s);
}

/*        FrBBlockStatement                     */

FrBBlockStatement::FrBBlockStatement()
{
}
    
void FrBBlockStatement::resolveAndCheck(FrBResolveEnvironment& e)
  throw (FrBResolveException)
{
  for(FrBStatementlist::iterator it = _stats.begin(); it != _stats.end(); ++it)
    (*it)->resolveAndCheck(e);

}

void FrBBlockStatement::execute(FrBExecutionEnvironment& e) const
  throw (FrBExecutionException)
{
  for(FrBStatementlist::const_iterator it = _stats.begin(); it != _stats.end(); ++it)
    (*it)->execute(e);
}
   
std::ostream& FrBBlockStatement::put(std::ostream& stream, int indent) const
{
  String str_indent(indent, '\t');

  stream << "execute:";

  indent += 2;

  for(FrBStatementlist::const_iterator it = _stats.begin(); it != _stats.end(); ++it)
  {
    stream << std::endl << str_indent << "\t*stat> ";
    (*it)->put(stream, indent);
  }

  return stream;
}
    
FrBBlockStatement::~FrBBlockStatement()
{
}

/*       FrBConditionalBlockStatement          */

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
  if(!FrBClass::areCompatibles(_cond->getClass(), FrBBool::getCppClass()))
    throw FrBIncompatibleClassException(_cond->getClass(), FrBBool::getCppClass());

  FrBBlockStatement::resolveAndCheck(e);

}

bool FrBElseIfStatement::evalCond(FrBExecutionEnvironment& e) const
  throw (FrBExecutionException)
{
  FrBBaseObject * o = FrBClass::forceConvert(_cond->eval(e), FrBBool::getCppClass());
  return (static_cast<FrBBool*>(o))->value();
}

std::ostream& FrBElseIfStatement::put(std::ostream& stream, int indent) const
{
  stream << "if (" << *_cond << ") ";

  return FrBBlockStatement::put(stream, indent);
}
    
FrBElseIfStatement::~FrBElseIfStatement()
{
}

/*         FrBElseStatement            */

bool FrBElseStatement::evalCond(FrBExecutionEnvironment& e) const throw (FrBExecutionException)
{
  return true;
}


/*       FrBIfStatement                   */

FrBIfStatement::FrBIfStatement()
{
}
    
void  FrBIfStatement::resolveAndCheck(FrBResolveEnvironment& e)
  throw (FrBResolveException)
{
  for(FrBCondList::iterator it = _conds.begin(); it != _conds.end(); ++it)
    (*it)->resolveAndCheck(e);
}

void  FrBIfStatement::execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException)
{
  FrBCondList::const_iterator it = _conds.begin();
  while( it != _conds.end() && (!((*it)->executeCond(e))) )
    ++it;
}

std::ostream&  FrBIfStatement::put(std::ostream& stream, int indent) const
{
  String str_indent(indent, '\t');

  FrBCondList::const_iterator it = _conds.begin();

  indent += 1;

  if(it != _conds.end())
  {
    (*it)->put(stream, indent);
    ++it;
  }

  while(it != _conds.end())
  {
    stream << std::endl << str_indent << "       else ";
    (*it)->put(stream, indent);
    ++it;
  }

  return stream << std::endl << str_indent << "       end if";
}
    
FrBIfStatement::~FrBIfStatement()
{
}

/*                 FrBDeclareStatement              */

FrBDeclareStatement::FrBDeclareStatement(int varid, FrBTypeExpr * t, FrBExpr * init_val)
    : _varid(varid), _type(t), _init(init_val)
{

}

void FrBDeclareStatement::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    _type->resolveAndCheck(e);
    
    if(_init)
    {
        _init->resolveAndCheck(e);
        if(!FrBClass::areCompatibles(_init->getClass(), _type->getClass()))
            throw FrBIncompatibleClassException(_init->getClass(), _type->getClass());
    }
}

void FrBDeclareStatement::execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException)
{

    e.stack().setTopValue(_varid,
                  ((_init == 0) ? _type->getClass()->createInstance(e) :
                                  FrBClass::forceConvert(_init->eval(e), _type->getClass())));
}

std::ostream& FrBDeclareStatement::put(std::ostream& stream, int) const
{
    stream << "declare <local_var:" << _varid << "> (" << *_type << ')';
    
    if(_init)
        stream << " with init value " << *_init;
    
    return stream;
}

FrBDeclareStatement::~FrBDeclareStatement()
{
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

std::ostream& FrBExprStatement::put(std::ostream& stream, int) const
{
    return stream << "Eval expression " << *_expr;
}

FrBExprStatement::~FrBExprStatement()
{
    delete _expr;
}



