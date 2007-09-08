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

#include "frbfield.h"
#include "frbexpr.h"
#include "frbclass.h"
#include "frbkeywords.h"
#include "frbexecutionenvironment.h"
#include "../common/misc.h"



std::ostream& operator<<(std::ostream& s, const FrBField& m)
{
    return m.put(s);
}

/*   FrBField    */

std::ostream& FrBField::put(std::ostream& stream, int indent) const
{
  return stream << FrBKeywords::scopeToString(scope()) << ' '
		<< FrBKeywords::sharedToString(shared()) << ' '
		<< name() << ' '
		<< FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_AS) << ' '
		<< type()->name() << std::endl;
}

FrBField * FrBField::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy) const
{
    frb_assert2(cpy, "could not specialize this field");
    return static_cast<FrBField*>(cpy);
}


/*     FrBCodeField        */

FrBCodeField::~FrBCodeField()
{
    delete_expr(_unresolvedType);
    delete_expr(_defaultVal);
}  
   
void FrBCodeField::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
  if(_defaultVal)
  {
    _defaultVal->resolveAndCheck(e);
    if(!_defaultVal->getClass()->isCompatibleWith(_unresolvedType->getClass()))
      throw FrBIncompatibleClassException(_defaultVal->getClass(), 
					  _unresolvedType->getClass());
  }
}

void FrBCodeField::resolvePrototype(FrBResolveEnvironment& e) throw (FrBResolveException)
{
  frb_assert(_unresolvedType);
  _unresolvedType->resolveAndCheck(e);
}
    
const FrBClass * FrBCodeField::type() const
{
  return _unresolvedType->getClass();
}


FrBBaseObject * FrBCodeField::evalDefaultValue(FrBExecutionEnvironment& e)
  const throw (FrBExecutionException)
{
  frb_assert(_unresolvedType);

  if(_defaultVal)
  {
    return FrBClass::forceConvert(_defaultVal->eval(e), _unresolvedType->getClass());
  }
  else
  {
     if(_unresolvedType->getClass()->inheritsFrom(container()))
       return FrBNull::nullValue();
     else
       return _unresolvedType->getClass()->createInstance(e);
  }
}

FrBCodeField * FrBCodeField::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy) const
{
    FrBCodeField * ret = static_cast<FrBCodeField *>(copy_not_null(cpy));

    e.setCurrentMember(ret);

    ret->_unresolvedType = static_cast<FrBTypeExpr*>(_unresolvedType->specializeTemplate(e));
    ret->_defaultVal = _defaultVal->specializeTemplate(e);

    return ret;
}

std::ostream& FrBCodeField::put(std::ostream& stream, int indent) const
{
  stream << FrBKeywords::scopeToString(scope()) << ' '
	 << FrBKeywords::sharedToString(shared()) << ' '
	 << name() << ' '
	 << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_AS) << ' '
	 << *_unresolvedType;

  if(_defaultVal)
    stream << ' ' << FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_ASSIGN_REF) 
	   << ' ' << *_defaultVal;

  return stream << " ' index=" << index() << std::endl;
}


    


