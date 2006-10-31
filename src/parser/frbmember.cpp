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


/*     FrBCodeMember        */

FrBCodeMember::~FrBCodeMember()
{
}  
   
void FrBCodeMember::resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException)
{
}

void FrBCodeMember::resolvePrototype(FrBResolveEnvironment&) throw (FrBResolveException)
{
}
    
const FrBClass * FrBCodeMember::type() const
{
}

FrBBaseObject * FrBCodeMember::evalDefaultValue(FrBExecutionEnvironment& e) const 
      throw (FrBExecutionException)
{
}
    


