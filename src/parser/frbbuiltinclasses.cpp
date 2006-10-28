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
#include "frbclass.h"
#include <iostream>


/*                   FrBObject                       */
FrBCppClass * FrBObject::_cpp_class = NULL;


FrBCppClass * FrBObject::initClass()
{
    frb_assert2(_cpp_class == NULL, "FrBObject::initClass(), already initialized");
    
    _cpp_class = new FrBCppClass(new FrBObject::Allocator());
    _cpp_class->setName("Object");
    _cpp_class->setSealed(true);
    _cpp_class->setScope(SC_PUBLIC);

    return _cpp_class;
}

FrBClass * FrBObject::getClass()
{
    frb_assert2(_cpp_class != NULL, "FrBObject::getClass(), call initClass() first");
    return _cpp_class;
}

FrBObject::~FrBObject()
{
}


/****************/

template<class primitive_t> FrBCppClass * FrBPrimitive<primitive_t>::_cpp_class = NULL;

/*            FrBInt            */

FrBBaseObject * operator_add_FrBInt(FrBExecutionEnvironment& e, FrBBaseObject * me, FrBBaseObject * arg0)
{
    FrBInt * r = new FrBInt((static_cast<FrBInt*>(me))->value() + (static_cast<FrBInt*>(arg0))->value());
    e.addGarbagedObject(r);
    return r;
}

FrBBaseObject * operator_mul_FrBInt(FrBExecutionEnvironment& e, FrBBaseObject * me, FrBBaseObject * arg0)
{
    FrBInt * r = new FrBInt((static_cast<FrBInt*>(me))->value() * (static_cast<FrBInt*>(arg0))->value());
    e.addGarbagedObject(r);
    return r;
}

FrBBaseObject * operator_affect_FrBInt(FrBExecutionEnvironment& e, FrBBaseObject * me, FrBBaseObject * arg0)
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


FrBBaseObject * operator_add_FrBString_FrBString(FrBExecutionEnvironment& e, FrBBaseObject * me, FrBBaseObject * arg0)
{
    FrBString * r =
        new FrBString((static_cast<FrBString*>(me))->value() + (static_cast<FrBString*>(arg0))->value());
    e.addGarbagedObject(r);
    return r;
}


FrBBaseObject * operator_add_FrBString_FrBInt(FrBExecutionEnvironment& e, FrBBaseObject * me, FrBBaseObject * arg0)
{
    FrBString * r =
        new FrBString((static_cast<FrBString*>(me))->value() +
                            StringEx::int2string( (static_cast<FrBInt*>(arg0))->value() ) );
    e.addGarbagedObject(r);
    return r;
}


FrBCppClass * FrBString::initClass()
{
    frb_assert2(_cpp_class == NULL, "FrBString::initClass(), already initialized");
    
    _cpp_class = new FrBCppClass(new FrBPrimitive::Allocator());
    _cpp_class->setName("String");
    _cpp_class->setSealed(true);
    _cpp_class->setScope(SC_PUBLIC);
    
    FrBFunction * f = new FrBUnaryCppFunction(operator_add_FrBString_FrBString, _cpp_class, false);
    f->setReturnType(_cpp_class);
    f->setName("operator+|&(String)");
    f->setConst(true);
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addOperator(FrBKeywords::FRB_KW_OP_ADD, f);
    _cpp_class->addOperator(FrBKeywords::FRB_KW_OP_CONCAT, f);
    
    f = new FrBUnaryCppFunction(operator_add_FrBString_FrBInt,  FrBInt::getCppClass(), false);
    f->setReturnType(_cpp_class);
    f->setName("operator+|&(Int)");
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

FrBBaseObject * function_call(FrBExecutionEnvironment& e, FrBBaseObject * me, const FrBBaseObjectList& p)
    throw (FrBExecutionException)
{
    
    FrBFunctionWrapper * fw = dynamic_cast<FrBFunctionWrapper*>(me);
    frb_assert(fw);
    
    const FrBFunction * f = fw->value();
    frb_assert(f);

    return f->execute(e, me, p);
}

FrBCppClass * FrBFunctionWrapper::initClass()
{
    frb_assert2(_cpp_class == NULL, "FrBFunctionWrapper::initClass(), already initialized");
    
    _cpp_class = new FrBCppClass(new FrBPrimitive::Allocator());
    _cpp_class->setName("Function");
    _cpp_class->setSealed(true);
    _cpp_class->setScope(SC_PUBLIC);
    
    FrBFunction * f = new FrBUnaryParamArrayCppFunction(function_call, FrBObject::getCppClass());
    f->setReturnType(FrBObject::getCppClass());
    f->setName("operator()");
    f->setConst(true);
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addOperator(FrBKeywords::FRB_KW_OP_O_BRACKET, f);

    return _cpp_class;
}



/*                   FrBDebug                       */
FrBCppClass * FrBDebug::_cpp_class = NULL;



template<class T>
FrBBaseObject * print_FrBObject(FrBExecutionEnvironment&, FrBBaseObject*, FrBBaseObject * arg0)
{

    frb_assert2(arg0, "null pointer passed to Debug::print()");
    
    std::cout << (static_cast<T*>(arg0))->value();

    return 0;
}

template<class T>
FrBBaseObject * println_FrBObject(FrBExecutionEnvironment&, FrBBaseObject*, FrBBaseObject * arg0)
{

    frb_assert2(arg0, "null pointer passed to Debug::println()");
    
    std::cout << (static_cast<T*>(arg0))->value() << std::endl;

    return 0;
}

template<class T>
FrBBaseObject * printwr_FrBObject(FrBExecutionEnvironment&, FrBBaseObject*, FrBBaseObject * arg0)
{
    //print with ref
    frb_assert2(arg0, "null pointer passed to Debug::printwr()");
    
    std::cout << '[' << (static_cast<T*>(arg0))->value()  << '@' << arg0 << ']' << std::endl;

    return 0;
}

FrBBaseObject * constructor1(FrBExecutionEnvironment&, FrBBaseObject * me)
{

    std::cout << "Debug.Initialize() called\n";

    return 0;
}

FrBBaseObject * show_stack(FrBExecutionEnvironment& e, FrBBaseObject * me)
{
    e.stack().print();
    return 0;
}

FrBBaseObject * show_mem(FrBExecutionEnvironment& e, FrBBaseObject * me)
{
    e.memory()->print(1);
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
    
    f = new FrBUnaryCppFunction(println_FrBObject<FrBInt>, FrBInt::getCppClass(), false);
    f->setName("println");
    f->setSub(true);
    f->setShared(true);
    f->setConst(true);
    
    _cpp_class->addFunction(f);
    
    f = new FrBUnaryCppFunction(println_FrBObject<FrBString>, FrBString::getCppClass(), false);
    f->setName("println");
    f->setSub(true);
    f->setShared(true);
    f->setConst(true);
    
    _cpp_class->addFunction(f);
    
    f = new FrBUnaryCppFunction(printwr_FrBObject<FrBInt>, FrBInt::getCppClass(), false);
    f->setName("printwr");
    f->setSub(true);
    f->setShared(true);
    f->setConst(true);
    
    _cpp_class->addFunction(f);
    
    f = new FrBUnaryCppFunction(printwr_FrBObject<FrBString>, FrBString::getCppClass(), false);
    f->setName("printwr");
    f->setSub(true);
    f->setShared(true);
    f->setConst(true);
    
    _cpp_class->addFunction(f);
    
    f = new FrBNoParamCppFunction(show_stack);
    f->setName("showStack");
    f->setConst(true);
    f->setSub(true);
    f->setShared(true);
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addFunction(f);
    
    f = new FrBNoParamCppFunction(show_mem);
    f->setName("showMem");
    f->setConst(true);
    f->setSub(true);
    f->setShared(true);
    f->setScope(SC_PUBLIC);
    
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


