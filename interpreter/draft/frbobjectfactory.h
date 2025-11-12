/***************************************************************************
 *   This file is part of the FrB project                                  *
 *                                                                         *
 *   FrB project                                                           *
 *                                                                         *
 *   Copyright (C) 2006 - 2010 by GUILLAUME Nicolas                        *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://frb.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/



#ifndef FRBOBJECTFACTORY_H
#define FRBOBJECTFACTORY_H

/* Object Factory */

#include <map>
#include <vector>
#include <cstdlib>
#include <iostream>
#include "string.h"

#define FRB_TYPE_DEC(type) FrBBaseObject * __objFactory__##type##Creator() \
                           { return new type(); } \
                           static ObjectFactory __objFactory__##type(type().typeName(), __objFactory__##type##Creator); \
                           int type::typeID() { return __objFactory__##type.getID(); }

#define FRB_OBJECT(name) public: \
                             virtual inline String typeName() { return name; } \
                             virtual int typeID(); \
                         private:

#define ARG(x) dynamic_cast<x>(args.at(i++))

#define FRB_EXPORT_BEGIN public: \
                         virtual FrBBaseObject * __objFactory__callFunction \
                                    (String fn, FrBArgList args, bool * fnNotFound = NULL) \
                         { if( fnNotFound != NULL ) { *fnNotFound = false; }

#define FRB_EXPORT_FN(fn_name, sign, ...) if( fn == #fn_name "(" sign ")" ) { \
                                             FrBArgList::size_type i = 0; (void)i; \
                                             return fn_name( __VA_ARGS__ ); }

#define FRB_EXPORT_SUB(fn_name, sign, ...) if( fn == #fn_name "(" sign ")" ) { \
                                             FrBArgList::size_type i = 0; (void)i; \
                                             fn_name( __VA_ARGS__ ); return NULL; }    
                                             
#define FRB_EXPORT_BASE(base) \
                        { bool test; \
                        FrBBaseObject * ret = base::__objFactory__callFunction(fn, args, &test); \
                        if(!test) { return ret; } }

#define FRB_EXPORT_END if( fnNotFound != NULL ) { *fnNotFound = true; } \
                       return NULL; } \
                       private:

class FrBBaseObject;
                       
typedef FrBBaseObject * (*FrBObjectCreator)();
typedef std::map<String, FrBObjectCreator> FrBCreatorList;
typedef std::map<String, std::map<String, void*> > FrBFunctionList;
typedef std::vector<FrBBaseObject*> FrBArgList;

class FrBBaseObject
{
    FRB_EXPORT_BEGIN;
    FRB_EXPORT_END;
    
public:
    inline FrBBaseObject() { }
    virtual ~FrBBaseObject() { }
    virtual int typeID() = 0;
    virtual String typeName() = 0;
};

static FrBCreatorList * ptrFrbTypes = NULL;

void __objFactory__destroy();

class ObjectFactory
{
private:
    int _id;

public:
    ObjectFactory(const String& name, FrBObjectCreator fn);
    static FrBBaseObject * newObject(const String& type);
    
    inline int getID() { return _id; }
    inline static FrBCreatorList * getTypeList() { return ptrFrbTypes; }
    inline static void destroyFactory() { __objFactory__destroy(); }
    inline static void deleteObject(FrBBaseObject* obj) { std::cout << "deleteObject()\n"; delete obj; }
    
    inline static FrBBaseObject* callFunction(FrBBaseObject * obj, String fn,
                                              FrBArgList args = FrBArgList(), bool * fnNotFound = NULL)
    { return obj->__objFactory__callFunction(fn, args, fnNotFound); }


    
    /*static FrBBaseObject* __callFunction(FrBBaseObject * obj, void * fn, FrBArgList args);*/
};


#endif
