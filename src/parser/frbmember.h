/***************************************************************************
 *   This file is part of the FrB project                                  *
 *                                                                         *
 *   FrB project                                                           *
 *                                                                         *
 *   Copyright (C) 2006, 2007 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://frb.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/


#ifndef FRBMEMBER_H
#define FRBMEMBER_H

#include "../common/assert.h"
#include "../common/string.h"
#include "frbconstants.h"

class FrBClass;

class FrBMember /* a frb member */
{
protected:
    FrBClass* _contClass;
    String    _name;
    bool      _shared;
    bool      _const; 
    scope_t   _scope;
    
public:

    FrBMember();
    virtual ~FrBMember() {};
    
    inline void setShared(bool v) { _shared = v; }
    inline bool shared() const { return _shared; }

    inline void setConst(bool v) { _const = v; }
    inline bool isConst() const { return _const; }
    
    inline scope_t scope() const { return _scope; }
    inline void setScope(scope_t v) { _scope = v; }
    
    inline void setName(const String& n) { _name = n; }
    inline const String& name() const { return _name; }

    /** Return the full name of this member, ie Module1.Module2.MyClass1 */
    String fullName() const;


    inline void setContainer(FrBClass* c);
    inline const FrBClass* container() const;
    inline FrBClass* containerPtr();

    /** Used in type resolution (resolve type of var in function code...)*/
    virtual void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException) = 0;

    /** Used in type resolution (resolve type of field, function prototype, ...) */
    virtual void resolvePrototype(FrBResolveEnvironment&) throw (FrBResolveException) = 0;

    virtual FrBMember * specializeTemplate(const FrBTemplateSpecializationEnvironment& e, FrBStatement * cpy = 0) const;
        
};






















/*                   inlined                   */


inline void FrBMember::setContainer(FrBClass* c)
{
    frb_assert(c);
    _contClass = c;
}

inline const FrBClass* FrBMember::container() const
{
    return _contClass;
}

inline FrBClass* FrBMember::containerPtr()
{
    return _contClass;
}

#endif


