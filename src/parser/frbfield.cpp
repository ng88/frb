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

#include "frbfield.h"
#include "frbexpr.h"
#include "frbclass.h"
#include "frbkeywords.h"

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



/*     FrBCodeField        */

FrBCodeField::~FrBCodeField()
{
}  
   
void FrBCodeField::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
  if(_defaultVal)
  {
    _defaultVal->resolveAndCheck(e);
    if(!FrBClass::areCompatibles(_defaultVal->getClass(), _unresolvedType->getClass()))
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
    /*TODO if(_unresolvedType->getClass()->inheritsFrom(container())
       return la_valeur_null;
     else*/
       return _unresolvedType->getClass()->createInstance(e);
  }
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

  return stream << std::endl;
}


    

