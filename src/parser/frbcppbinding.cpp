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
#include "../common/misc.h"

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

FrBCppClass * FrBCppClass::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy) const
{
    frb_assert2(false, "templates not fully implemented in cpp binging");
    FrBCppClass * ret = static_cast<FrBCppClass*>(copy_not_null(cpy));

    FrBClass::specializeTemplate(e, ret);

    return ret;
}

/*       FrBCppFunction          */

FrBCppFunction * FrBCppFunction::specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy) const
{
    frb_assert2(false, "templates not fully implemented in cpp binging");
    return const_cast<FrBCppFunction*>(this);
}
    

/*           FrBCppObject             */

FrBCppObject::~FrBCppObject()
{
}

/*          FrBNoParamCppFunction         */

const FrBClass * FrBNoParamCppFunction::parameterType(size_t index) const
{
    return 0;
}

bool FrBNoParamCppFunction::parameterByVal(size_t index) const
{
    return false;
}

size_t FrBNoParamCppFunction::parameterCount() const
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


const FrBClass * FrBUnaryCppFunction::parameterType(size_t index) const
{
    return _arg0;
}

bool FrBUnaryCppFunction::parameterByVal(size_t index) const
{
    return _arg0_byval;
}

size_t FrBUnaryCppFunction::parameterCount() const
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


const FrBClass * FrBBinaryCppFunction::parameterType(size_t index) const
{
    return index ? _arg1 : _arg0;
}

bool FrBBinaryCppFunction::parameterByVal(size_t index) const
{
    return index ? _arg1_byval : _arg0_byval;
}

size_t FrBBinaryCppFunction::parameterCount() const
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

/*FrBBinaryCppFunction * FrBBinaryCppFunction::specializeTemplate(FrBTemplateSpecializationEnvironment& e, FrBMember * cpy) const
{
    return static_cast<FrBBinaryCppFunction*>(copy_not_null(cpy));
}*/


/*             FrBNaryCppFunction              */

    typedef FrBBaseObject * (*NaryFunction)(FrBExecutionEnvironment&, FrBBaseObject *, const FrBBaseObjectList& args);

const FrBClass * FrBNaryCppFunction::parameterType(size_t index) const
{
    frb_assert(index < _size);
    return _arg_class[index];
}

bool FrBNaryCppFunction::parameterByVal(size_t index) const
{
    frb_assert(index < _size);
    return _arg_byval[index];
}

size_t FrBNaryCppFunction::parameterCount() const
{
    return _size;
}

    
FrBBaseObject * FrBNaryCppFunction::execute(FrBExecutionEnvironment&e, FrBBaseObject * me, const FrBBaseObjectList& args) const
    throw (FrBExecutionException)
{
    frb_assert(args.size() == _size);
    return _fn(e, me, args);
}

/*FrBNaryCppFunction *  FrBNaryCppFunction::specializeTemplate(FrBTemplateSpecializationEnvironment& e, FrBMember * cpy) const
{
    return static_cast<FrBNaryCppFunction*>(copy_not_null(cpy));
}*/



/*          FrBUnaryParamArrayCppFunction          */


const FrBClass * FrBUnaryParamArrayCppFunction::parameterType(size_t index) const
{
    return _arg0;
}

bool FrBUnaryParamArrayCppFunction::parameterByVal(size_t index) const
{
    return false;
}

size_t FrBUnaryParamArrayCppFunction::parameterCount() const
{
    return 1;
}


FrBBaseObject * FrBUnaryParamArrayCppFunction::execute(FrBExecutionEnvironment& e, FrBBaseObject * me,
                                                const FrBBaseObjectList& args) const
     throw (FrBExecutionException)
{
    return _fn(e, me, args);
}

/*FrBUnaryParamArrayCppFunction * FrBUnaryParamArrayCppFunction::specializeTemplate(FrBTemplateSpecializationEnvironment& e, FrBMember * cpy) const
{
    return static_cast<FrBUnaryParamArrayCppFunction*>(copy_not_null(cpy));
}*/




