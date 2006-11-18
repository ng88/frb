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


#include "frbresolveenvironment.h"
#include "frbclass.h"
#include "../common/assert.h"

FrBClass * FrBResolveEnvironment::getClassFromName(const String& name) throw (FrBClassNotFoundException)
{
    FrBClassMap::iterator f = _root->find(name);
    
    if(f == _root->end())
        throw FrBClassNotFoundException(name);
    else
    {
        frb_assert(f->second);
        return f->second;
    }
}

FrBClass * FrBResolveEnvironment::getClassFromPath(const String& name) throw (FrBClassNotFoundException)
{
    //TODO
    return getClassFromName(name);
}

