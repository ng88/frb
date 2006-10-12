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


#ifndef FRBCPPBINDING_H
#define FRBCPPBINDING_H

#include "frbclass.h"
#include "frbbaseobject.h"
#include "../common/assert.h"

/** Classes and declarations that allows using C++ classes in FrB */


/** Un objet FrB du binding C++ */
class FrBCppObject : public FrBBindingObject
{

public:
    virtual ~FrBCppObject();
};

/** Allocator that allows creation of C++ objects */

class FrBCppObjectAllocator
{
public:
    virtual FrBCppObject * createObject() const = 0;
};


/** FrB class from C++ binding */
class FrBCppClass : public FrBClass
{
private:

    FrBCppObjectAllocator * _allocator;

protected:

    FrBBaseObject * allocateInstance(FrBExecutionEnvironment&) const throw (FrBAllocationException);
    void freeInstance(FrBExecutionEnvironment& e, FrBBaseObject * o) const throw (FrBAllocationException);

public:
    inline FrBCppClass(FrBCppObjectAllocator * a)
       : _allocator(a) { frb_assert2(a != 0, "FrBCppClass::FrBCppClass(), null FrBCppObjectAllocator pointer"); };
    
    virtual ~FrBCppClass();
    
    
    

    
    const char* specString() const;

};


/* cf p521*/

/** Une fonction C++ exportée */
class FrBCppFunction : public FrBFunction
{
private:
public:
};

class FrBNoParamCppFunction : public FrBCppFunction
{
public:

    typedef FrBBaseObject * (*NoParamFunction)(FrBBaseObject *);
    
private:

    NoParamFunction _fn;

public:

    FrBNoParamCppFunction(NoParamFunction f) : _fn(f) {};

    const FrBClass * parameterType(int index) const;
    bool parameterByVal(int index) const;
    int parameterCount() const;
    
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me) const
        throw (FrBExecutionException);
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException);
};

class FrBUnaryCppFunction : public FrBCppFunction
{
    
public:

    typedef FrBBaseObject * (*UnaryFunction)(FrBBaseObject *, FrBBaseObject *);
    
private:

    UnaryFunction _fn;
    FrBClass * _arg0;
    bool       _arg0_byval;
    
public:

    FrBUnaryCppFunction(UnaryFunction fn, FrBClass *  arg0, bool arg0_byval)
        : _fn(fn), _arg0(arg0), _arg0_byval(arg0_byval) {}

    const FrBClass * parameterType(int index) const;
    bool parameterByVal(int index) const;
    int parameterCount() const;
    
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me, FrBBaseObject * arg0) const
        throw (FrBExecutionException);
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException);
};



#endif


