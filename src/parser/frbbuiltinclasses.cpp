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


#include "frbbuiltinclasses.h"
#include "../common/assert.h"

#include "frbkeywords.h"

#include <iostream>

template<class primitive_t> FrBCppClass * FrBPrimitive<primitive_t>::_cpp_class = NULL;

/*            FrBInt            */

FrBBaseObject * operator_add_FrBInt(FrBBaseObject * me, FrBBaseObject * arg0)
{
    return new FrBInt((static_cast<FrBInt*>(me))->value() + (static_cast<FrBInt*>(arg0))->value());
}

FrBBaseObject * operator_mul_FrBInt(FrBBaseObject * me, FrBBaseObject * arg0)
{
    return new FrBInt((static_cast<FrBInt*>(me))->value() * (static_cast<FrBInt*>(arg0))->value());
}

FrBBaseObject * operator_affect_FrBInt(FrBBaseObject * me, FrBBaseObject * arg0)
{
    (static_cast<FrBInt*>(me))->setValue( (static_cast<FrBInt*>(arg0))->value() );
    return me;
}

FrBCppClass * FrBInt::initClass()
{
    frb_assert2(_cpp_class == NULL, "FrBInt::initClass(), already initialized");
    
    _cpp_class = new FrBCppClass(new FrBPrimitive::Allocator());
    _cpp_class->setName("Int");
    //_cpp_class->setShared(false);
    //_cpp_class->setAbstract(false);
    _cpp_class->setSealed(true);
    _cpp_class->setScope(SC_PUBLIC);
    
    FrBFunction * f = new FrBUnaryCppFunction(operator_add_FrBInt, _cpp_class, false);
    f->setReturnType(_cpp_class);
    f->setName("operator+(FrBint)");
    f->setConst(true);
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addOperator(FrBKeywords::FRB_KW_OP_ADD, f);
    
    f = new FrBUnaryCppFunction(operator_mul_FrBInt, _cpp_class, false);
    f->setReturnType(_cpp_class);
    f->setName("operator*(FrBint)");
    f->setConst(true);
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addOperator(FrBKeywords::FRB_KW_OP_MUL, f);
    
    f = new FrBUnaryCppFunction(operator_affect_FrBInt, _cpp_class, false);
    f->setReturnType(_cpp_class);
    f->setName("operator=(FrBint)");
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addOperator(FrBKeywords::FRB_KW_OP_ASSIGN_VAL, f);

    //TODO ajouter _cpp_class au module frb::std
    
    return _cpp_class;
}



/*            FrBString            */

FrBBaseObject * operator_add_FrBString(FrBBaseObject * me, FrBBaseObject * arg0)
{
    return new FrBString((static_cast<FrBString*>(me))->value() + (static_cast<FrBString*>(arg0))->value());
}


FrBCppClass * FrBString::initClass()
{
    frb_assert2(_cpp_class == NULL, "FrBString::initClass(), already initialized");
    
    _cpp_class = new FrBCppClass(new FrBPrimitive::Allocator());
    _cpp_class->setName("String");
    _cpp_class->setSealed(true);
    _cpp_class->setScope(SC_PUBLIC);
    
    FrBFunction * f = new FrBUnaryCppFunction(operator_add_FrBString, _cpp_class, false);
    f->setReturnType(_cpp_class);
    f->setName("operator+/&(FrBString)");
    f->setConst(true);
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addOperator(FrBKeywords::FRB_KW_OP_ADD, f);
    _cpp_class->addOperator(FrBKeywords::FRB_KW_OP_CONCAT, f);

    return _cpp_class;
}




/*            FrBClassWrapper            */

FrBCppClass * FrBClassWrapper::initClass()
{
    frb_assert2(_cpp_class == NULL, "FrBClassWrapper::initClass(), already initialized");
    
    _cpp_class = new FrBCppClass(new FrBPrimitive::Allocator());
    _cpp_class->setName("Class");
    _cpp_class->setSealed(true);
    _cpp_class->setScope(SC_PUBLIC);

    return _cpp_class;
}


/*            FrBFunctionWrapper            */

FrBCppClass * FrBFunctionWrapper::initClass()
{
    frb_assert2(_cpp_class == NULL, "FrBFunctionWrapper::initClass(), already initialized");
    
    _cpp_class = new FrBCppClass(new FrBPrimitive::Allocator());
    _cpp_class->setName("Function");
    _cpp_class->setSealed(true);
    _cpp_class->setScope(SC_PUBLIC);

    return _cpp_class;
}



/*                   FrBDebug                       */
FrBCppClass * FrBDebug::_cpp_class = NULL;


FrBBaseObject * constructor1(FrBBaseObject * me)
{

    std::cout << "Debug.Initialize() called\n";

    return 0;
}

template<class T>
FrBBaseObject * print_FrBObject(FrBBaseObject*, FrBBaseObject * arg0)
{

    std::cout << (static_cast<T*>(arg0))->value();

    return 0;
}




FrBCppClass * FrBDebug::initClass()
{
    frb_assert2(_cpp_class == NULL, "FrBDebug::initClass(), already initialized");
    
    _cpp_class = new FrBCppClass(new FrBDebug::Allocator());
    _cpp_class->setName("Debug");
    _cpp_class->setSealed(true);
    _cpp_class->setScope(SC_PUBLIC);
    

    FrBCppFunction * f = new FrBNoParamCppFunction(constructor1);
    f->setName("$init");
    f->setSub(true);
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addConstructor(f);
    
    f = new FrBUnaryCppFunction(print_FrBObject<FrBInt>, FrBInt::getCppClass(), false);
    f->setName("print");
    f->setSub(true);
    f->setShared(true);
    f->setConst(true);
    
    _cpp_class->addFunction(f);
    
    f = new FrBUnaryCppFunction(print_FrBObject<FrBString>, FrBString::getCppClass(), false);
    f->setName("print");
    f->setSub(true);
    f->setShared(true);
    f->setConst(true);
    
    _cpp_class->addFunction(f);
    

    
    return _cpp_class;
}

FrBClass * FrBDebug::getClass()
{
    frb_assert2(_cpp_class != NULL, "FrBDebug::getClass(), call initClass() first");
    return _cpp_class;
}

FrBDebug::~FrBDebug()
{
}


