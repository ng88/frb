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
    
    FrBResolveEnvironment(const FrBResolveEnvironment&) {}
    
public:

    /**
      * @param root Class root
      */
    FrBResolveEnvironment(FrBClassMap * root)
     : _root(root)
    {
    }
    
    inline FrBClassMap * classRoot() { return _root; }
    
    /** can resolve things like "String" or "Int"... */
    FrBClass * getClassFromName(const String& name) throw (FrBClassNotFoundException);
    
    /** can resolve things like "FrB.String" or "Module1.Application1.Classe1" or "Int"... */
    FrBClass * getClassFromPath(const String& name) throw (FrBClassNotFoundException);

    
    
    

};


#endif
