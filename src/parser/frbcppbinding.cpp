#include "frbcppbinding.h"

/*       FrBCppClass          */

FrBCppClass::~FrBCppClass()
{
    delete _allocator;
}

FrBBaseObject * FrBCppClass::allocateInstance() const throw (FrBAllocationException)
{
    return 0;//_allocator.createObject();
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


FrBBaseObject * FrBNoParamCppFunction::execute(FrBBaseObject * me) const
        throw (FrBExecutionException)
{
    return _fn(me);
}

FrBBaseObject * FrBNoParamCppFunction::execute(FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException)
{
    return execute(me);
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


FrBBaseObject * FrBUnaryCppFunction::execute(FrBBaseObject * me, FrBBaseObject * arg0) const
     throw (FrBExecutionException)
{
    return _fn(me, arg0);
}


FrBBaseObject * FrBUnaryCppFunction::execute(FrBBaseObject * me, const FrBBaseObjectList& args) const
     throw (FrBExecutionException)
{
    return _fn(me, args[0]);
    //execute(me, args[0]);
}



