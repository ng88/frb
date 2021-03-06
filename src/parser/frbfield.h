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


#ifndef FRBFIELD_H
#define FRBFIELD_H

#include "frbexceptions.h"
#include "../common/assert.h"


#include "frbmember.h"
#include "frbexprlist.h"

class FrBClass;
class FrBExpr;
class FrBTypeExpr;
class FrBExecutionEnvironment;
class FrBResolveEnvironment;

class FrBField : public FrBMember /* a frb field */
{
protected:
    int _index;
    
public:

    inline FrBField() : _index(-1)  {}

    virtual ~FrBField() {}
    virtual std::ostream& put(std::ostream& stream, int indent = 0) const;

    inline void setIndex(int v) { _index = v; }

    /** Return the field index.
     *  Warning: shared and non shared fields are counted independently, ie in a same class,
     *  a shared field can have the same index as a non shared field.
     */
    inline int index() const { frb_assert(_index > -1); return _index; }

    virtual const FrBClass * type() const = 0;

    /** Return the default value of the field, me can be obtained by e.stack().top() */
    virtual FrBBaseObject * evalDefaultValue(FrBExecutionEnvironment& e) const
        throw (FrBExecutionException) = 0;
    

    FRB_ASSERT_CODE(
	void setShared(bool v)
        {
	    frb_assert2(!container(), "field index can't be changed when container is set");
	    FrBMember::setShared(v); 
	} 
	           );

    FrBField * specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy = 0) const;
};

std::ostream& operator<<(std::ostream& s, const FrBField& m);


class FrBCodeField : public FrBField
{    
protected:
    
    FrBTypeExpr*         _unresolvedType;
    FrBExpr*             _defaultVal;
    
public:
    inline FrBCodeField(FrBTypeExpr * type, FrBExpr * init = 0);
    ~FrBCodeField();
    
    inline void setURType(FrBTypeExpr*);
    inline void setInitExpr(FrBExpr * e);
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    void resolvePrototype(FrBResolveEnvironment&) throw (FrBResolveException);
    
    const FrBClass * type() const;

    FrBBaseObject * evalDefaultValue(FrBExecutionEnvironment& e) const 
      throw (FrBExecutionException);

    FrBCodeField * specializeTemplate(/*const*/ FrBTemplateSpecializationEnvironment& e, FrBMember * cpy = 0) const;    

    std::ostream& put(std::ostream& stream, int indent = 0) const;

};




















/* Inlined */

inline FrBCodeField::FrBCodeField(FrBTypeExpr * type, FrBExpr * init)
  : _unresolvedType(type), _defaultVal(init)
{
}

inline void FrBCodeField::setURType(FrBTypeExpr* t)
{
    _unresolvedType = t;
}

inline void FrBCodeField::setInitExpr(FrBExpr * e)
{
    _defaultVal = e;
}

#endif


