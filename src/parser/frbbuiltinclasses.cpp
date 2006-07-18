#include "frbbuiltinclasses.h"
#include "../common/assert.h"

#include "frbkeywords.h"

#include <iostream>

template<class primitive_t> FrBCppClass * FrBPrimitive<primitive_t>::_cpp_class = NULL;

/*            FrBInt            */

FrBBaseObject * operator_add_FrBInt(FrBBaseObject * me, FrBBaseObject * arg0)
{
    int a = (static_cast<FrBInt*>(me))->value();
    int b = (static_cast<FrBInt*>(arg0))->value();
    
    std::cout << "operator_add_FrBInt(" << a << ", " << b << ")\n";
    
    return new FrBInt(a + b);
}

FrBBaseObject * operator_mul_FrBInt(FrBBaseObject * me, FrBBaseObject * arg0)
{
    return new FrBInt((static_cast<FrBInt*>(me))->value() * (static_cast<FrBInt*>(arg0))->value());
}

FrBCppClass * FrBInt::initClass()
{
    frb_assert2(_cpp_class == NULL, "FrBInt:initClass(), already initialized");
    
    _cpp_class = new FrBCppClass(new FrBPrimitive::Allocator());
    _cpp_class->setName("Int");
    //_cpp_class->setShared(false);
    //_cpp_class->setAbstract(false);
    _cpp_class->setSealed(true);
    _cpp_class->setScope(SC_PUBLIC);
    
    FrBFunction * f = new FrBUnaryCppFunction(operator_add_FrBInt, _cpp_class, false);
    f->setReturnType(_cpp_class);
    f->setName("operator+(FrBint)");
    f->setSub(false);
    f->setShared(false);
    f->setConst(true);
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addOperator(FrBKeywords::FRB_KW_OP_ADD, f);
    
    f = new FrBUnaryCppFunction(operator_mul_FrBInt, _cpp_class, false);
    f->setReturnType(_cpp_class);
    f->setName("operator*(FrBint)");
    f->setSub(false);
    f->setShared(false);
    f->setConst(true);
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addOperator(FrBKeywords::FRB_KW_OP_MUL, f);

    
    return _cpp_class;
}



/*            FrBString            */

FrBBaseObject * operator_add_FrBString(FrBBaseObject * me, FrBBaseObject * arg0)
{
    return new FrBString((static_cast<FrBString*>(me))->value() + (static_cast<FrBString*>(arg0))->value());
}


FrBCppClass * FrBString::initClass()
{
    frb_assert2(_cpp_class == NULL, "FrBString:initClass(), already initialized");
    
    _cpp_class = new FrBCppClass(new FrBPrimitive::Allocator());
    _cpp_class->setName("String");
    _cpp_class->setSealed(true);
    _cpp_class->setScope(SC_PUBLIC);
    
    FrBFunction * f = new FrBUnaryCppFunction(operator_add_FrBString, _cpp_class, false);
    f->setReturnType(_cpp_class);
    f->setName("operator+(FrBString)");
    f->setConst(true);
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addOperator(FrBKeywords::FRB_KW_OP_ADD, f);

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
    frb_assert2(_cpp_class == NULL, "FrBDebug:initClass(), already initialized");
    
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


