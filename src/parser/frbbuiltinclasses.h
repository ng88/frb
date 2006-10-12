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


#ifndef FRBBUILTINCLASSES_H
#define FRBBUILTINCLASSES_H


#include "frbcppbinding.h"
#include "frbmemory.h"

template<class primitive_t>
class FrBPrimitive : public FrBCppObject
{
private:
    primitive_t _val;
    static FrBCppClass * _cpp_class;

public:

    static FrBCppClass * initClass();
    
    inline FrBPrimitive() : _val(0) {};
    inline FrBPrimitive(const primitive_t& v) : _val(v) {};
    
    ~FrBPrimitive() {}
    
    inline void setValue(const primitive_t& v) { _val = v; }
    inline const primitive_t& value() { return _val; }
    
    
    
    inline static FrBClass * getCppClass()
    {
        frb_assert2(_cpp_class != NULL, "FrBInt::getClass(), call initClass() first");
        return _cpp_class;
    }
    

    FrBClass * getClass() { return getCppClass(); }
    
    
    class Allocator : public FrBCppObjectAllocator
    {
      public:
        FrBCppObject * createObject() const { return new FrBPrimitive<primitive_t>(); }
    };
    
};

typedef FrBPrimitive<int> FrBInt;
typedef FrBPrimitive<String> FrBString;

typedef FrBPrimitive<const FrBClass*> FrBClassWrapper;
typedef FrBPrimitive<const FrBFunction*> FrBFunctionWrapper;




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
    
    class Allocator : public FrBCppObjectAllocator
    {
      public:
        FrBCppObject * createObject() const { return new FrBDebug(); }
    };
        
    
};



#endif


