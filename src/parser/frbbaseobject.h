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


#ifndef FRBBASEOBJECT_H
#define FRBBASEOBJECT_H

#include <map>
#include <vector>
#include "../common/string.h"

class FrBClass;

class FrBBaseObject
{

protected:

    bool     _const;
    
    
public:
    inline FrBBaseObject() : _const(false) { }
    inline FrBBaseObject(bool v) : _const(v) { }
    
    virtual ~FrBBaseObject() { }
    virtual const FrBClass * getClass() = 0;
    
    //inline void setConst(bool v) { _const = v; }
    inline bool isConst() { return _const; }

};

typedef std::vector<FrBBaseObject*> FrBBaseObjectList;
typedef std::map<String,FrBBaseObject*> FrBBaseObjectMap;

/* Un objet FrB utilisateur */
class FrBObject : public FrBBaseObject
{
     //FRB_OBJECT("Object");
     
    /** Object data*/
    FrBBaseObjectList _data; //a-t-on besoin des noms (map) ? a priori non
    const FrBClass * _class;
    
public:
    inline FrBObject(const FrBClass * c, int size)
     : _data(size), _class(c) {}
     
    virtual ~FrBObject();
    const FrBClass * getClass();

};

/* Un objet FrB issu d'un binding FrB */
class FrBBindingObject : public FrBBaseObject
{

public:
    
    virtual ~FrBBindingObject();

};

#endif

