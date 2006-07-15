#ifndef FRBBUILTINCLASSES_H
#define FRBBUILTINCLASSES_H


#include "frbcppbinding.h"
#include "frbmemory.h"

class FrBInt : public FrBCppObject
{
private:
    int _val;
    static FrBCppClass * _cpp_class;

public:

    static FrBCppClass * initClass();
    
    inline FrBInt() : _val(0) {};
    inline FrBInt(const int& v) : _val(v) {};
    
    inline void setValue(int v) { _val = v; }
    inline int value() { return _val; }
    
    virtual ~FrBInt();
    
    FrBClass * getClass();
    
    inline static FrBClass * getCppClass()
    {
        frb_assert2(_cpp_class != NULL, "FrBInt::getClass(), call initClass() first");
        return _cpp_class;
    }
    

    
    
};
class FrBIntAllocator : public FrBCppObjectAllocator
{
    FrBCppObject * createObject() const { return new FrBInt(); }
};






class FrBDebug : public FrBCppObject
{
private:
    static FrBCppClass * _cpp_class;

public:

    static FrBCppClass * initClass();
    
    inline FrBDebug()  {};
    

    virtual ~FrBDebug();
    
    FrBClass * getClass();
    
    inline static FrBClass * getCppClass()
    {
        frb_assert2(_cpp_class != NULL, "FrBDebug::getClass(), call initClass() first");
        return _cpp_class;
    }
    

    
    
};
class FrBDebugAllocator : public FrBCppObjectAllocator
{
    FrBCppObject * createObject() const { return new FrBDebug(); }
};


#endif


