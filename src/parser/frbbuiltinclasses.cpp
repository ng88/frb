#include "frbbuiltinclasses.h"
#include "../common/assert.h"

#include "frbkeywords.h"

#include <iostream>

/*            FrBInt            */
FrBCppClass * FrBInt::_cpp_class = NULL;


FrBBaseObject * operator_add_FrBInt(FrBBaseObject * me, FrBBaseObject * arg0)
{
    int a = (static_cast<FrBInt*>(me))->value();
    int b = (static_cast<FrBInt*>(arg0))->value();
    
    std::cout << "operator_add_FrBInt_FrBInt(" << a << ", " << b << ")\n";
    
    return new FrBInt(a + b);
}


FrBCppClass * FrBInt::initClass()
{
    frb_assert2(_cpp_class == NULL, "FrBInt:initClass(), already initialized");
    
    _cpp_class = new FrBCppClass(new FrBIntAllocator());
    _cpp_class->setName("Int");
    //_cpp_class->setShared(false);
    //_cpp_class->setAbstract(false);
    _cpp_class->setSealed(true);
    _cpp_class->setScope(SC_PUBLIC);
    
    FrBFunction * f = new FrBUnaryCppFunction(operator_add_FrBInt, FrBInt::getCppClass(), false);
    f->setReturnType(FrBInt::getCppClass());
    f->setName("operator+(FrBint)");
    f->setSub(false);
    f->setShared(false);
    f->setConst(true);
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addOperator(FrBKeywords::FRB_KW_OP_ADD, f);

    
    return _cpp_class;
}

FrBClass * FrBInt::getClass()
{
    frb_assert2(_cpp_class != NULL, "FrBInt::getClass(), call initClass() first");
    return _cpp_class;
}

FrBInt::~FrBInt()
{
}




/*                   FrBDebug                       */
FrBCppClass * FrBDebug::_cpp_class = NULL;


FrBBaseObject * constructor1(FrBBaseObject * me)
{

    std::cout << "Debug.Initialize() called\n";

    return 0;
}

FrBBaseObject * print_FrBInt(FrBBaseObject * me, FrBBaseObject * arg0)
{

    std::cout << (static_cast<FrBInt*>(arg0))->value();

    return 0;
}




FrBCppClass * FrBDebug::initClass()
{
    frb_assert2(_cpp_class == NULL, "FrBDebug:initClass(), already initialized");
    
    _cpp_class = new FrBCppClass(new FrBIntAllocator());
    _cpp_class->setName("Debug");
    //_cpp_class->setShared(false);
    //_cpp_class->setAbstract(false);
    _cpp_class->setSealed(true);
    _cpp_class->setScope(SC_PUBLIC);
    

    FrBCppFunction * f = new FrBNoParamCppFunction(constructor1);
    f->setName("$init");
    f->setSub(true);
    f->setShared(false);
    f->setConst(false);
    f->setScope(SC_PUBLIC);
    
    _cpp_class->addConstructor(f);
    
    f = new FrBUnaryCppFunction(print_FrBInt, FrBInt::getCppClass(), false);
    f->setName("print");
    f->setSub(true);
    f->setShared(false);
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


