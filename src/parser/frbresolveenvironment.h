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


#ifndef FRBRESOLVEENVIRONMENT_H
#define FRBRESOLVEENVIRONMENT_H

#include "frbfunction.h"
class FrBClass;


/** This class contains the resolve environment (class tree) */
class FrBResolveEnvironment
{
private:

    FrBClassMap * _root;
    FrBClassMap _importedClass;
    
    FrBResolveEnvironment(const FrBResolveEnvironment&) {}
    
public:

    /**
      * @param root Class root
      */
    FrBResolveEnvironment(FrBClassMap * root)  : _root(root) { }
    
    inline FrBClassMap * classRoot() { return _root; }
    
    /** can resolve things like "String" or "Int"... (look for imported (if lookForImported)  & root classes) */
    const FrBClass * getClassFromName(const String& name, FrBClass * parent = 0, bool lookForImported = true)
        throw (FrBClassNotFoundException);
 
    
    /** can resolve things like "FrB.String" or "Module1.Application1.Classe1" or "Int"... */
    const FrBClass * getClassFromPath(const String& name) throw (FrBClassNotFoundException);
    
    /** add an imported class or a class shortcut*/
    void addImportedClass(const String& fullPath, const String& importName = "") throw (FrBClassNotFoundException);

        

};


#endif
