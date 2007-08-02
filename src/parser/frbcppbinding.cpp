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


#include "frbcppbinding.h"
#include "frbbuiltinclasses.h"
#include "../common/assert.h"

/*       FrBCppClass          */

FrBCppClass::~FrBCppClass()
{
    delete _allocator;
}

FrBBaseObject * FrBCppClass::allocateInstance(FrBExecutionEnvironment& e) const throw (FrBAllocationException)
{
    FrBBaseObject * o = _allocator->createObject();
    e.addGarbagedObject(o);
    return o;
}


void FrBCppClass::freeInstance(FrBExecutionEnvironment& e, FrBBaseObject * o) const
                throw (FrBAllocationException)
{
    delete o;
}


const CString FrBCppClass::specString() const
{
    return "C++ binding class";
}
    

/*           FrBCppObject             */

FrBCppObject::~FrBCppObject()
{
}

/*          FrBNoParamCppFunction         */

const FrBClass * FrBNoParamCppFunction::parameterType(int index) const
{
    return 0;
}

bool FrBNoParamCppFunction::parameterByVal(int index) const
{
    return false;
}

int FrBNoParamCppFunction::parameterCount() const
{
    return 0;
}


FrBBaseObject * FrBNoParamCppFunction::execute(FrBExecutionEnvironment& e, FrBBaseObject * me) const
        throw (FrBExecutionException)
{
    return _fn(e, me);
}

FrBBaseObject * FrBNoParamCppFunction::execute(FrBExecutionEnvironment& e, FrBBaseObject * me,
                                                const FrBBaseObjectList& args) const
        throw (FrBExecutionException)
{
    return execute(e, me);
}




/*          FrBUnaryCppFunction          */


const FrBClass * FrBUnaryCppFunction::parameterType(int index) const
{
    return _arg0;
}

bool FrBUnaryCppFunction::parameterByVal(int index) const
{
    return _arg0_byval;
}

int FrBUnaryCppFunction::parameterCount() const
{
    return 1;
}


FrBBaseObject * FrBUnaryCppFunction::execute(FrBExecutionEnvironment& e, FrBBaseObject * me,
                                                FrBBaseObject * arg0) const
     throw (FrBExecutionException)
{
    return _fn(e, me, arg0);
}


FrBBaseObject * FrBUnaryCppFunction::execute(FrBExecutionEnvironment& e, FrBBaseObject * me,
                                                const FrBBaseObjectList& args) const
     throw (FrBExecutionException)
{
    frb_assert(args.size() == 1);
    return _fn(e, me, args[0]);

}

/*          FrBBinaryCppFunction          */


const FrBClass * FrBBinaryCppFunction::parameterType(int index) const
{
    return index ? _arg1 : _arg0;
}

bool FrBBinaryCppFunction::parameterByVal(int index) const
{
    return index ? _arg1_byval : _arg0_byval;
}

int FrBBinaryCppFunction::parameterCount() const
{
    return 2;
}


FrBBaseObject * FrBBinaryCppFunction::execute(FrBExecutionEnvironment& e, FrBBaseObject * me,
                                                FrBBaseObject * arg0, FrBBaseObject * arg1) const
     throw (FrBExecutionException)
{
    return _fn(e, me, arg0, arg1);
}


FrBBaseObject * FrBBinaryCppFunction::execute(FrBExecutionEnvironment& e, FrBBaseObject * me,
                                                const FrBBaseObjectList& args) const
     throw (FrBExecutionException)
{
    frb_assert(args.size() == 2);
    return _fn(e, me, args[0], args[1]);

}

/*             FrBNaryCppFunction              */

    typedef FrBBaseObject * (*NaryFunction)(FrBExecutionEnvironment&, FrBBaseObject *, const FrBBaseObjectList& args);

const FrBClass * FrBNaryCppFunction::parameterType(int index) const
{
    frb_assert(index < _size);
    return _arg_class[index];
}

bool FrBNaryCppFunction::parameterByVal(int index) const
{
    frb_assert(index < _size);
    return _arg_byval[index];
}

int FrBNaryCppFunction::parameterCount() const
{
    return _size;
}

    
FrBBaseObject * FrBNaryCppFunction::execute(FrBExecutionEnvironment&e, FrBBaseObject * me, const FrBBaseObjectList& args) const
    throw (FrBExecutionException)
{
    frb_assert(args.size() == _size);
    return _fn(e, me, args);
}



/*          FrBUnaryParamArrayCppFunction          */


const FrBClass * FrBUnaryParamArrayCppFunction::parameterType(int index) const
{
    return _arg0;
}

bool FrBUnaryParamArrayCppFunction::parameterByVal(int index) const
{
    return false;
}

int FrBUnaryParamArrayCppFunction::parameterCount() const
{
    return 1;
}


FrBBaseObject * FrBUnaryParamArrayCppFunction::execute(FrBExecutionEnvironment& e, FrBBaseObject * me,
                                                const FrBBaseObjectList& args) const
     throw (FrBExecutionException)
{
    return _fn(e, me, args);
}



