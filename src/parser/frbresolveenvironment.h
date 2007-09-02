/***************************************************************************
 *   This file is part of the FrB project                                  *
 *                                                                         *
 *   FrB project                                                           *
 *                                                                         *
 *   Copyright (C) 2006, 2007 by GUILLAUME Nicolas                         *
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

/* Templates:
 *  1) at parse time: we associate a fake class to each template parameter
 *                    and we do not resolveAndCheck
 *  2) at resolve time: 1) if a template is used for the first time, we clone
 *                      its class and replace the fake class by the real class
 *                      (parameters). then the new class is resolved an added
 *                      to the class tree.
 *                      2) we already have resolved the class so we can use
 *                      it directly.
 */

/** This class contains the resolve environment (class tree) */
class FrBResolveEnvironment
{
public:
    //!!! draft !!!
    /** Fake class class */
    /*class FrBTemplateClass
    {
	std::vector<FrBClass*> array;
	std::map<FrBClass*, int> assoc;
	FrBClass * getFake(int i);
	int getFakeNumber(const FrBClass*);
	};*/
    
    /** give the association argument_number -> new_real_class for the current remplate */
    //typedef std::stack<FrBConstClassList> FrBTemplateStack

private:

    FrBClassMap * _root;
    FrBClassMap _importedClass;
    
    FrBResolveEnvironment(const FrBResolveEnvironment&) {}

    /** return container[name] if exists or 0 if not */
    static FrBClass * findClass(const String& name, FrBClassMap * container);
    
public:

    /**
      * @param root Class root
      */
    FrBResolveEnvironment(FrBClassMap * root)  : _root(root) { }
    
    inline FrBClassMap * classRoot() { return _root; }
 
    
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

};


#endif
