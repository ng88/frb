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



FrBClass * FrBResolveEnvironment::getClassFromPath(const String& name) throw (FrBClassNotFoundException)
{
    StringList elm;
    FrBClass * current = 0;
    
    StringEx::split(elm, FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_MEMBER), false);
    
    StringList::const_iterator it = elm.begin();
    if(it != elm.end())
    {
        current = getClassFromName(*it);
        it++;
    }
    
    while(it != elm.end())
    {
        current = getNextClassFromName(*it, current);
        it++;
    }
    
    if(!current)
        throw FrBClassNotFoundException(name);
        
    return current;
}

FrBClass * FrBResolveEnvironment::getNextClassFromName(const String& name, FrBClass * parent)
    throw (FrBClassNotFoundException)
{
    FrBClassMap * inners = parent->innerClassPtr();
    
    FrBClassMap::iterator f = inners->find(name);
    
    if(f == inners->end())
        throw FrBClassNotFoundException(name);
    else
    {
        frb_assert(f->second);
        return f->second;
    }
}

FrBClass * FrBResolveEnvironment::findClass(const String& name, FrBClassMap * container)
{
    FrBClassMap::iterator f = container->find(name);
    if(f != container->end())
    {
        frb_assert(f->second);
        return f->second;
    }
    
    return 0;
}

FrBClass * FrBResolveEnvironment::getClassFromName(const String& name, FrBClass * context)
    throw (FrBClassNotFoundException)
{
    FrBClass * ret = 0;
    
    /* look for local class (outer) */
    
    if( context && context->containerPtr() &&
            (ret = findClass(name, context->containerPtr()->innerClassPtr())) )
        return ret;
        
    /* look for local class (inner) */
    
    if( context && (ret = findClass(name, context->innerClassPtr())) )
        return ret;
    
    /* look for imported class */
    
    if( (ret = findClass(name, &_importedClass)) )
        return ret;
        
    /* look for class in root */
    
    if( (ret = findClass(name, _root)) )
        return ret;
    
    throw FrBClassNotFoundException(name);
}


void FrBResolveEnvironment::addImportedClass(const String& fullPath, const String& importName)
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


