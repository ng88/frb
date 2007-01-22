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


#ifndef FRBCPPBINDING_H
#define FRBCPPBINDING_H

#include "frbclass.h"
#include "frbbaseobject.h"
#include "../common/assert.h"

/* Classes and declarations that allows using C++ classes in FrB */


/** Represents an FrB object that is an instance of a C++ binding type */
class FrBCppObject : public FrBBindingObject
{

public:
    /** We don't care about fields in c++ binding */
    FrBBaseObject* getField(int) throw (FrBEvaluationException) { return 0; }
    /** We don't care about fields in c++ binding */
    void addField(int, FrBBaseObject *) throw (FrBEvaluationException) {}

    virtual ~FrBCppObject();
};

/** Allocator that allows creation of C++ objects */
class FrBCppObjectAllocator
{
public:
    /** Create an instance */
    virtual FrBCppObject * createObject() const = 0;
};


/** Represents an FrB class from C++ binding */
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
    
    
    

    
    const CString specString() const;

};


/* cf p521*/

/** Represents an FrB function that is a C++ exported function */
class FrBCppFunction : public FrBFunction
{
private:
public:
};

/** Represents an FrB function that is a C++ exported function without any parameter
  *      ie fn()
  */
class FrBNoParamCppFunction : public FrBCppFunction
{
public:

    typedef FrBBaseObject * (*NoParamFunction)(FrBExecutionEnvironment&, FrBBaseObject *);
    
private:

    NoParamFunction _fn;

public:

    /** 'f' is a pointer to the C++ function */
    FrBNoParamCppFunction(NoParamFunction f) : _fn(f) {};

    const FrBClass * parameterType(int index) const;
    bool parameterByVal(int index) const;
    int parameterCount() const;
    
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me) const
        throw (FrBExecutionException);
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException);
};

/** Represents an FrB function that is a C++ exported function with one parameter
  *      ie fn( ByVal/ByRef a1 As T)
  */
class FrBUnaryCppFunction : public FrBCppFunction
{
    
public:

    typedef FrBBaseObject * (*UnaryFunction)(FrBExecutionEnvironment&, FrBBaseObject *, FrBBaseObject *);
    
private:

    UnaryFunction _fn;
    FrBClass * _arg0;
    bool       _arg0_byval;
    
public:

    /** 'fn' is a pointer to the C++ function, 'arg0' is the type of the first argument and 'arg0_byval' indicates if the first arg must be passed byval or not (=byref)  */
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

/** fn(ParamArray a As T) */
class FrBUnaryParamArrayCppFunction : public FrBCppFunction
{
    
public:

    typedef FrBBaseObject * (*UnaryParamArray)(FrBExecutionEnvironment&, FrBBaseObject*, const FrBBaseObjectList &);
    
private:

    UnaryParamArray _fn;
    FrBClass * _arg0;
    
public:

    FrBUnaryParamArrayCppFunction(UnaryParamArray fn, FrBClass *  arg0)
        : _fn(fn), _arg0(arg0) { setParamArrayUsed(true); }

    const FrBClass * parameterType(int index) const;
    bool parameterByVal(int index) const;
    int parameterCount() const;
    
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException);
};



#endif


