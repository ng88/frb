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


#ifndef FRBFIELD_H
#define FRBFIELD_H

#include "frbexceptions.h"
#include "../common/assert.h"

#include "frbexecutionenvironment.h"

#include "frbexprlist.h"
#include "frbconstants.h"

class FrBClass;
class FrBExpr;
class FrBTypeExpr;

class FrBField /* a frb field */
{
protected:
    bool _shared;
    bool _const; 
    String _name;
    scope_t  _scope;
    
public:

    inline FrBField() : _shared(false), _const(false), _scope(SC_PUBLIC)  {}

    virtual ~FrBField() {}
    virtual std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    inline void setShared(bool v) { _shared = v; }
    inline bool shared() const { return _shared; }
    
    inline void setConst(bool v) { _const = v; }
    inline bool isConst() const { return _const; }
    
    inline scope_t scope() const { return _scope; }
    inline void setScope(scope_t v) { _scope = v; }
    
    inline void setName(const String& n) { _name = n; }
    inline const String& name() const { return _name; }

    virtual const FrBClass * type() const = 0;

    /** Return the default value of the field */
    virtual FrBBaseObject * evalDefaultValue(FrBExecutionEnvironment& e) const
        throw (FrBExecutionException) = 0;
    
    
    /** Used in type resolution */
    virtual void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException) {};
    virtual void resolvePrototype(FrBResolveEnvironment&) throw (FrBResolveException) {};
        
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
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    void resolvePrototype(FrBResolveEnvironment&) throw (FrBResolveException);
    
    const FrBClass * type() const;

    FrBBaseObject * evalDefaultValue(FrBExecutionEnvironment& e) const 
      throw (FrBExecutionException);
    
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

#endif


