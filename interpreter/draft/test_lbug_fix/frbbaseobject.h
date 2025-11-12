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


#ifndef FRBBASEOBJECT_H
#define FRBBASEOBJECT_H

#include <map>
#include <vector>
#include "../common/string.h"
#include "../common/assert.h"
#include "frbexceptions.h"
#include "frbfield.h"

class FrBClass;



/** Represents a general FrB object (ie an instance of a FrB Class) */
class FrBBaseObject
{
public:

    enum { NULL_OBJECT = 0, UNMANAGED_OBJECT = -2 };

    typedef unsigned char frbmem_info_t;

    enum { STD_MEM_AREA = 0, CST_MEM_AREA = 1, TMP_MEM_AREA = 2};

protected:

  //bool     _const;
    int                 _mem_pos;
    
    /** Type information (memory type, ...) */
    frbmem_info_t     _mem_info;
    
    

public:

    inline FrBBaseObject() : /*_const(false),*/_mem_pos(UNMANAGED_OBJECT), _mem_info(STD_MEM_AREA) { }
    //    inline FrBBaseObject(bool v) : _const(v) { }
    
    virtual ~FrBBaseObject() { }
    
    /** Return object type */
    virtual const FrBClass * getClass() = 0;

    /** Return the instance of the field i */
    virtual FrBBaseObject* getField(int i) const throw (FrBEvaluationException) = 0;

    /** Add field o */
    virtual void addField(int i, FrBBaseObject * o) throw (FrBEvaluationException) = 0;

    /** Overloaded function, provided for convenience */
    inline FrBBaseObject* getField(const FrBField * f) const throw (FrBEvaluationException);

    /** Overloaded function, provided for convenience */
    inline void addField(const FrBField * f, FrBBaseObject * o);
    
    //inline void setConst(bool v) { _const = v; }
    //inline bool isConst() { return _const; }


    /** Memory information
     * STD_MEM_AREA: standard (managed heap)
     * CST_MEM_AREA: constant area (where are stored literal values)
     * TMP_MEM_AREA: temporary area (where are stored intermediate result)
     */
    inline void setMemInfo(frbmem_info_t infos) { _mem_info = infos; }
    inline frbmem_info_t memInfo() const { return _mem_info; }
    inline bool isInConstantArea() const { return (_mem_info & CST_MEM_AREA); }
    inline bool isInTemporaryArea() const { return (_mem_info & TMP_MEM_AREA); }
    inline bool isInStandardArea() const { return (_mem_info & STD_MEM_AREA); }

    /** Duplicate current instance, return Null if not implemented */
    virtual FrBBaseObject * duplicate() const;
    
    /** used by the garbage collector */
    inline void setMemPos(int v) { _mem_pos = v; }
    inline int memPos() const { return _mem_pos; }
    inline bool isManaged() const { return _mem_pos != UNMANAGED_OBJECT; }


};

/** Vector of FrBBaseObject* */
typedef std::vector<FrBBaseObject*> FrBBaseObjectList;

/** Map of FrBBaseObject* with a String key */
typedef std::map<String,FrBBaseObject*> FrBBaseObjectMap;


/** Represents an FrB object that is an instance of an user type (ie defined by the interpreter) */
class FrBUserObject : public FrBBaseObject
{
private:

    FRB_ASSERT_CODE(int _field_count);

protected:

    const FrBClass * _type;
    FrBBaseObject ** _fields;

public:
    
    FrBUserObject(const FrBClass * type, int fc);
    
    const FrBClass * getClass() { return _type; }

    FrBBaseObject* getField(int) const throw (FrBEvaluationException);
    void addField(int, FrBBaseObject *) throw (FrBEvaluationException);
     

    ~FrBUserObject();


};

/** Represents an FrB object that is an instance of a binding type */
class FrBBindingObject : public FrBBaseObject
{

public:
    
    virtual ~FrBBindingObject();

};

















/********* Inlined ************/




inline FrBBaseObject* FrBBaseObject::getField(const FrBField * f) const throw (FrBEvaluationException)
{
    frb_assert2(!f->shared(), "f MUST be a non-shared field");
    return getField(f->index());
}

inline void FrBBaseObject::addField(const FrBField * f, FrBBaseObject * o)
{
    frb_assert2(!f->shared(), "f MUST be a non-shared field");
    addField(f->index(), o);
}








#endif


