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

std::ostream& operator<<(std::ostream& s, const FrBField& m)
{
    return m.put(s);
}

/*   FrBField    */

std::ostream& FrBField::put(std::ostream& stream, int indent) const
{
  //TODO
  return stream << "field " << _name;
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


FrBBaseObject * FrBCodeField::evalDefaultValue(FrBExecutionEnvironment& e) const 
      throw (FrBExecutionException)
{
  frb_assert(_unresolvedType);

  if(_defaultVal)
    return FrBClass::forceConvert(_defaultVal->eval(e), _unresolvedType->getClass());
  else
    return _unresolvedType->getClass()->createInstance(e);
}
    


