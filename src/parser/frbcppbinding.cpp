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


#include "frbcppbinding.h"

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


const char* FrBCppClass::specString() const
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
    return _fn(e, me, args[0]);
    //execute(me, args[0]);
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



