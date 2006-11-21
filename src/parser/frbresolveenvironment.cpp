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
//il faudrait une fonction simple a utiliser que pour la recherche de chemin complet pour getClassFromPath
//et  getClassFromName serait utilisé pour la premiere etape de getClassFromPath et devrait en plus
// s'occuper de la recherches des classes du module courant (ie container()->innerClassList())

const FrBClass * FrBResolveEnvironment::getClassFromName(const String& name, FrBClass * parent,
        bool lookForImported) throw (FrBClassNotFoundException)
{
    FrBClassMap::iterator f;
    
    if(lookForImported)
    {
        f = _importedClass.find(name);
        
        if(f != _importedClass.end())
        {
            frb_assert(f->second);
            return f->second;
        }
    }

    FrBClassMap * inners = parent ? parent->innerClassPtr() : _root;
    
    f = inners->find(name);
    
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
    FrBClassMap * current = 0;
    
    StringEx::split(elm, FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_MEMBER), false);
    
    StringList::const_iterator it = elm.begin();
    if(it != elm.end())
    {
        current = getClassFromName(*it, current, elm.size() == 1);
        it++;
    }
    
    while(it != elm.end())
    {
        current = getClassFromName(*it, current, false);
        it++;
    }
    
    if(!current)
        throw FrBClassNotFoundException(name);
        
    return current;
}

void FrBResolveEnvironment::addImportedClass(const String& fullPath, const String& importName = "")
    throw (FrBClassNotFoundException)
{
    try
    {
        FrBClass * c = getClassFromPath(fullPath);
        
        _importedClass[ (importName == "") ? c->name() : importName ] = c;
    }
    catch(FrBClassNotFoundException ex)
    {
        //TODO on cherche la classe ds les chemins d'import et on charge si c'est bon
        //si on trouve, arbre complet ds root et raccourci dans _importedClass
        throw ex;
    }

}


