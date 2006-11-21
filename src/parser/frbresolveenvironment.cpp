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
#include "../common/string.h"
#include "frbkeywords.h"

const FrBClass * FrBResolveEnvironment::getClassFromName(const String& name, FrBClass * parent) throw (FrBClassNotFoundException)
{
    const FrBClassMap * inners = parent ? parent->innerClassList() : _root;
    
    FrBClassMap::const_iterator f = inners->find(name);
    
    if(f == inners->end())
        throw FrBClassNotFoundException(name);
    else
    {
        frb_assert(f->second);
        return f->second;
    }
}

const FrBClass * FrBResolveEnvironment::getClassFromPath(const String& name) throw (FrBClassNotFoundException)
{
    StringList elm;
    const FrBClassMap * current = 0;
    
    StringEx::split(elm, FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_MEMBER), false);
    
    for(StringList::const_iterator it = elm.begin(); it != elm.end(); ++it)
        current = getClassFromName(*it, current);
    
    if(!current)
        throw FrBClassNotFoundException(name);
        
    return current;
}

