
#ifndef FRBTYPES_H
#define FRBTYPES_H

#include "string.h"
#include "frbmem.h"
#include <iostream>


class FrBObject : public FrBBaseObject
{
    FRB_OBJECT("Object");
    
public:
    inline FrBObject()
    {
    }
    
    virtual ~FrBObject()
    {
    }

};

class FrBInt : public FrBObject
{
    FRB_OBJECT("Int");
    
    FRB_EXPORT_BEGIN;
    FRB_EXPORT_FN( operatorAdd, "Int", ARG(FrBInt*) );
    FRB_EXPORT_FN( operatorMult, "Int", ARG(FrBInt*) );
    FRB_EXPORT_END;
    
private:
    int _val;

public:
    inline FrBInt() : _val(0) {}
    inline FrBInt(int i) : _val(i) {}
    inline FrBInt(FrBInt& o) : _val(o._val) {}
    inline FrBInt(FrBInt* o) : _val(o->_val) {}
    
    ~FrBInt()
    {
    }

    FrBBaseObject * operatorAdd(FrBInt * a)
    {
        static FrBInt ret(0);
        ret._val = _val + a->_val;
        return &ret;
    }

    FrBBaseObject * operatorMult(FrBInt * a)
    {
        static FrBInt ret(0);
        ret._val = _val * a->_val;
        return &ret;
    }

};


class FrBString : public FrBObject
{
    FRB_OBJECT("String");
    
private:
    String _chaine;
    
public:
    FrBString()
    {
    }
    
    ~FrBString()
    {
    }

};


class FrBFunction : public FrBObject
{
    FRB_OBJECT("Function");

public:
    FrBFunction()
    {
    }
    
    ~FrBFunction()
    {
    }
        
};

class FrBUserClass : public FrBObject
{
    FRB_OBJECT("__user_class");

public:
    FrBUserClass()
    {
    }
    
    ~FrBUserClass()
    {
    }
        
};

class FrBConsole : public FrBObject
{
    FRB_OBJECT("Console");
    
    FRB_EXPORT_BEGIN;
    FRB_EXPORT_SUB( write, "String", ARG(FrBString*) );
    FRB_EXPORT_SUB( write, "Int", ARG(FrBInt*) );
    FRB_EXPORT_FN( readInt, "" );
    FRB_EXPORT_FN( readString, "" );
    FRB_EXPORT_END;
    
public:
    void write(FrBString * str);
    void write(FrBInt * str);
    
    FrBInt* readInt();
    FrBString* readString();
};

class FrBTest : public FrBObject
 {
        
    FRB_OBJECT("Test");
    
    FRB_EXPORT_BEGIN;
    FRB_EXPORT_SUB( fn_test, "String,String,Int", ARG(FrBString*), ARG(FrBString*), ARG(FrBInt*) );
    FRB_EXPORT_BASE(FrBObject);
    FRB_EXPORT_END;
    
 public:
    FrBTest();
    
    ~FrBTest()
    {
    }
    
    void fn_test(FrBString * arg1, FrBString * arg2, FrBInt * arg3);
        
 };
 
 

#endif

