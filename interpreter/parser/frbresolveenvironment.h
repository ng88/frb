/***************************************************************************
 *   This file is part of the FrB project                                  *
 *                                                                         *
 *   FrB project                                                           *
 *                                                                         *
 *   Copyright (C) 2006 - 2010 by GUILLAUME Nicolas                        *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://frb.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/


#ifndef FRBRESOLVEENVIRONMENT_H
#define FRBRESOLVEENVIRONMENT_H

#include "frbfunction.h"
class FrBClass;


/** This class contains the resolve environment (class tree) */
class FrBResolveEnvironment
{
public:

private:

    FrBClassMap * _root;
    FrBClassMap _importedClass;
    FrBClassMap _templateInstances;
    
    FrBResolveEnvironment(const FrBResolveEnvironment&) {}

    /** return container[name] if exists or 0 if not */
    static FrBClass * findClass(const String& name, FrBClassMap * container);
    
public:
    
    /**
      * @param root Class root
      */
    FrBResolveEnvironment(FrBClassMap * root)  : _root(root) { }
    
    inline FrBClassMap * classRoot() { return _root; }
    inline FrBClassMap * templates() { return &_templateInstances; }
 
    
    /** can resolve things like "FrB.String" or "Module1.Application1.Classe1" or "Int"... */
    FrBClass * getClassFromPath(const String& name)
        throw (FrBClassNotFoundException);
        
    /** can resolve things like "String" or "Int"... (look for imported (if lookForImported)  & root classes)
      * context, if specified, is the class in which name has been found
      */
    FrBClass * getClassFromName(const String& name, FrBClass * context = 0)
        throw (FrBClassNotFoundException);
    
    /** can resolve things like parent.name where name is a type name */
    static FrBClass * getNextClassFromName(const String& name, FrBClass * parent)
        throw (FrBClassNotFoundException);
    
    /** add an imported class or a class shortcut*/
    void addImportedClass(const String& fullPath, const String& importName = "")
        throw (FrBClassNotFoundException);


    inline void addTemplate(FrBClass * c, const String& name);

    inline FrBClass* findTemplate(const String& name);

};











/*            inlined         */

inline void FrBResolveEnvironment::addTemplate(FrBClass * c, const String& name)
{
    _templateInstances.insert(std::make_pair(name, c));
}


inline FrBClass * FrBResolveEnvironment::findTemplate(const String& name)
{
    FrBClassMap::const_iterator f = _templateInstances.find(name);
    return (f == _templateInstances.end()) ? 0 : f->second;
}


#endif
