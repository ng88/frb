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
    FrBBaseObject* getField(int) const throw (FrBEvaluationException) { return 0; }
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
    virtual ~FrBCppObjectAllocator() {}
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
    
        
    FrBCppClass * specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy = 0) const;
    
    const CString specString() const;

};


/* cf p521*/

/** Represents an FrB function that is a C++ exported function */
class FrBCppFunction : public FrBFunction
{
private:
public:
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException) {}
    void resolvePrototype(FrBResolveEnvironment&) throw (FrBResolveException) {}

    FrBCppFunction * specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy = 0) const;

};

/** Function that does nothing */
class FrBNopCppFunction : public FrBCppFunction
{

public:

    FrBNopCppFunction() {}

    const FrBClass * parameterType(size_t index) const { return 0; }
    bool parameterByVal(size_t index) const { return false; }
    size_t parameterCount() const { return 0; }
    
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject *) const
        throw (FrBExecutionException) { return 0; }
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject *, const FrBBaseObjectList&) const
        throw (FrBExecutionException) { return 0; }

    //FrBCppFunction * specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy = 0) const;
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
    FrBNoParamCppFunction(NoParamFunction f) : _fn(f) { frb_assert(f); }

    const FrBClass * parameterType(size_t index) const;
    bool parameterByVal(size_t index) const;
    size_t parameterCount() const;
    
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me) const
        throw (FrBExecutionException);
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException);

    //FrBCppFunction * specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy = 0) const;
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
        : _fn(fn), _arg0(arg0), _arg0_byval(arg0_byval) { frb_assert(fn && arg0); }

    const FrBClass * parameterType(size_t index) const;
    bool parameterByVal(size_t index) const;
    size_t parameterCount() const;
    
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me, FrBBaseObject * arg0) const
        throw (FrBExecutionException);
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException);

    //FrBCppFunction * specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy = 0) const;

};

class FrBBinaryCppFunction : public FrBCppFunction
{
    
public:

    typedef FrBBaseObject * (*BinaryFunction)(FrBExecutionEnvironment&, FrBBaseObject *, FrBBaseObject *, FrBBaseObject *);
    
private:

    BinaryFunction _fn;
    FrBClass * _arg0;
    bool       _arg0_byval;
    FrBClass * _arg1;
    bool       _arg1_byval;
    
public:

    /** 'fn' is a pointer to the C++ function, 'arg0' is the type of the first argument and 'arg0_byval' indicates if the first arg must be passed byval or not (=byref)  */
    FrBBinaryCppFunction(BinaryFunction fn, FrBClass *  arg0, bool arg0_byval, FrBClass *  arg1, bool arg1_byval)
        : _fn(fn), _arg0(arg0), _arg0_byval(arg0_byval), _arg1(arg1), _arg1_byval(arg1_byval) { frb_assert(fn && arg0 && arg1); }

    const FrBClass * parameterType(size_t index) const;
    bool parameterByVal(size_t index) const;
    size_t parameterCount() const;
    
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me, FrBBaseObject * arg0, FrBBaseObject * arg1) const
        throw (FrBExecutionException);
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException);

    //FrBCppFunction * specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy = 0) const;

};


class FrBNaryCppFunction : public FrBCppFunction
{
    
public:

    typedef FrBBaseObject * (*NaryFunction)(FrBExecutionEnvironment&, FrBBaseObject *, const FrBBaseObjectList& args);
    
private:

    NaryFunction _fn;
    FrBClass **  _arg_class;
    bool      *  _arg_byval;
    size_t       _size;

public:

    /** arg_class & arg_byval arrays are not freed */
    FrBNaryCppFunction(NaryFunction fn, FrBClass ** arg_class, bool * arg_byval, size_t size)
        : _fn(fn), _arg_class(arg_class), _arg_byval(arg_byval), _size(size) { frb_assert(fn && arg_byval && arg_class && size >= 0); }

    //~FrBNaryCppFunction();

    const FrBClass * parameterType(size_t index) const;
    bool parameterByVal(size_t index) const;
    size_t parameterCount() const;
    
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException);

    //FrBNaryCppFunction * specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy = 0) const;

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

    const FrBClass * parameterType(size_t index) const;
    bool parameterByVal(size_t index) const;
    size_t parameterCount() const;
    
    FrBBaseObject * execute(FrBExecutionEnvironment&, FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException);

    //FrBUnaryParamArrayCppFunction * specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy = 0) const;

};



#endif


