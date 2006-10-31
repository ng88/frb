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

#include "frbmember.h"

std::ostream& operator<<(std::ostream& s, const FrBMember& m)
{
    return m.put(s);
}

/*   FrBMember    */

std::ostream& FrBMember::put(std::ostream& stream, int indent) const
{
  //TODO
  return stream << "member " << _name;
}



/*     FrBCodeMember        */

FrBCodeMember::~FrBCodeMember()
{
}  
   
void FrBCodeMember::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
  if(_defaultVal)
  {
    _defaultVal->resolveAndCheck(e);
    if(!FrBClass::areCompatibles(_defaultVal->getClass(), _unresolvedType->getClass()))
      throw FrBIncompatibleClassException(_defaultVal->getClass(), 
					  _unresolvedType->getClass());
  }
}

void FrBCodeMember::resolvePrototype(FrBResolveEnvironment& e) throw (FrBResolveException)
{
  frb_assert(_unresolvedType);
  _unresolvedType->resolveAndCheck(e);
}
    
const FrBClass * FrBCodeMember::type() const
{
  return _unresolvedType->getClass();
}


FrBBaseObject * FrBCodeMember::evalDefaultValue(FrBExecutionEnvironment& e) const 
      throw (FrBExecutionException)
{
  frb_assert(_unresolvedType);

  if(_defaultVal)
    return FrBClass::forceConvert(_defaultVal->eval(e), _unresolvedType->getClass());
  else
    return _unresolvedType->getClass()->createInstance(e);
}
    


