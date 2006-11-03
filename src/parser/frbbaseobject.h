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

  //bool     _const;
    int      _mem_pos;
    
    
public:

    enum { NULL_OBJECT = 0, UNMANAGED_OBJECT = -2 };

    inline FrBBaseObject() : /*_const(false),*/_mem_pos(UNMANAGED_OBJECT) { }
    //    inline FrBBaseObject(bool v) : _const(v) { }
    
    virtual ~FrBBaseObject() { }
    virtual const FrBClass * getClass() = 0;

    /** return the instance of the field i */
    virtual FrBBaseObject* getField(int i) = 0;

    /** add field o */
    virtual void addField(FrBBaseObject * o) = 0;
    
    //inline void setConst(bool v) { _const = v; }
    //inline bool isConst() { return _const; }
    
    /** used by the garbage collector */
    inline void setMemPos(int v) { _mem_pos = v; }
    inline int memPos() const { return _mem_pos; }
    inline bool isManaged() const { return _mem_pos != UNMANAGED_OBJECT; }


};

typedef std::vector<FrBBaseObject*> FrBBaseObjectList;
typedef std::map<String,FrBBaseObject*> FrBBaseObjectMap;


/** user instance */
class FrBUserObject : public FrBBaseObject
{

protected:

    const FrBClass * _type;
    FrBBaseObject * _fields[];

public:
    
    FrBUserObject(const FrBClass * type, int fc) : _type(type) {}
    
    const FrBClass * getClass() { return _type; }
     
    virtual ~FrBUserObject() {}

};

/* Un objet FrB issu d'un binding FrB */
class FrBBindingObject : public FrBBaseObject
{

public:
    
    virtual ~FrBBindingObject();

};

#endif

