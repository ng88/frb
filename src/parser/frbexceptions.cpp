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


#include "frbexceptions.h"
#include "frbkeywords.h"
#include "frberrors.h"
#include "frbbaseobject.h"
#include "frbclass.h"
#include "frbmember.h"
#include "frbexpr.h"
#include "../common/assert.h"

std::ostream& operator<<(std::ostream& s, const FrBException& c)
{
    return c.put(s);
}

/*               FrBInvalidLValueException                          */

FrBInvalidLValueException::FrBInvalidLValueException(const String& lvalue)
 : _lvalue(lvalue)
{
}

FrBInvalidLValueException::FrBInvalidLValueException(const FrBExpr* lvalue)
{
    std::ostringstream sstr;
    lvalue->put(sstr);
    _lvalue = sstr.str();
}

std::ostream& FrBInvalidLValueException::put(std::ostream& stream) const
{
    return FrBErrors::putMsg(stream, FrBErrors::FRB_ERR_INVALID_LVALUE, _lvalue);
}


/*             FrBUndeclaredIdentifierException                */

FrBUndeclaredIdentifierException::FrBUndeclaredIdentifierException(const String& name)
  : _name(name)
{
}

std::ostream& FrBUndeclaredIdentifierException::put(std::ostream& stream) const
{
    return stream << _name << ": " << FrBErrors::getMsg(FrBErrors::FRB_ERR_UNDECL_ID);
}

/*                FrBStackOverflowException                      */

std::ostream& FrBStackOverflowException::put(std::ostream& stream) const
{
    return stream;
}

/*          FrBFieldNotFoundException           */

FrBFieldNotFoundException::FrBFieldNotFoundException(const String& name)
  : FrBMemberNotFoundException(name)
{
}

std::ostream& FrBFieldNotFoundException::put(std::ostream& stream) const
{
  return FrBErrors::putMsg(stream, FrBErrors::FRB_ERR_FIELD_NOT_FOUND, _name);
}
    

/*             FrBFunctionNotFoundException                */


FrBFunctionNotFoundException::FrBFunctionNotFoundException(const String& name)
    : FrBMemberNotFoundException(name)
{
}

FrBFunctionNotFoundException::FrBFunctionNotFoundException(const String& name, const std::vector<const FrBClass*>& args)
    : FrBMemberNotFoundException(name), _args(args)
{
}

FrBFunctionNotFoundException::FrBFunctionNotFoundException(const String& name, const std::vector<FrBBaseObject*>& args)
    : FrBMemberNotFoundException(name)
{
    for(std::vector<FrBBaseObject*>::const_iterator it = args.begin(); it != args.end(); ++it)
        _args.push_back( (*it)->getClass() );
}

FrBFunctionNotFoundException::FrBFunctionNotFoundException(const String& name, const FrBExprList& args)
    : FrBMemberNotFoundException(name)
{
    for(FrBExprList::const_iterator it = args.begin(); it != args.end(); ++it)
        _args.push_back( (*it)->getClass() );
}


std::ostream& FrBFunctionNotFoundException::put(std::ostream& stream) const
{
  return FrBErrors::putMsg(stream, FrBErrors::FRB_ERR_FN_NOT_FOUND, _name);
}


/*             FrBFunctionAmbiguityException                */

FrBFunctionAmbiguityException::FrBFunctionAmbiguityException(const String& name)
    : FrBFunctionNotFoundException(name)
{
}

std::ostream& FrBFunctionAmbiguityException::put(std::ostream& stream) const
{
    return FrBErrors::putMsg(stream, FrBErrors::FRB_ERR_FN_AMBIGUITY, _name);
}



/*                   FrBDefaultCtorNotFoundException                */


FrBDefaultCtorNotFoundException::FrBDefaultCtorNotFoundException(const FrBClass * c)
    : FrBFunctionNotFoundException(FrBKeywords::getKeyword(FrBKeywords::FRB_KW_CONSTRUCTOR_NAME)), _c(c)
{
}

/*                   FrBDtorNotFoundException                */


FrBDtorNotFoundException::FrBDtorNotFoundException()
 : FrBFunctionNotFoundException(FrBKeywords::getKeyword(FrBKeywords::FRB_KW_DESTRUCTOR_NAME))
{

}


/*             FrBClassNotFoundException             */

FrBClassNotFoundException::FrBClassNotFoundException(const String& name)
    : FrBMemberNotFoundException(name)
{
}

std::ostream& FrBClassNotFoundException::put(std::ostream& stream) const
{
  return FrBErrors::putMsg(stream, FrBErrors::FRB_ERR_CLASS_NOT_FOUND, _name);
}


/*               FrBIncompatibleClassException                    */


FrBIncompatibleClassException::FrBIncompatibleClassException(const FrBClass * provided, const FrBClass * needed)
    : _provided(provided), _needed(needed)
{

}

std::ostream& FrBIncompatibleClassException::put(std::ostream& stream) const
{
    return stream;
}


/*        FrBMemberNotFoundException           */


FrBMemberNotFoundException::FrBMemberNotFoundException(const String& name)
 :  _name(name)
{
}


std::ostream& FrBMemberNotFoundException::put(std::ostream& stream) const
{
  return FrBErrors::putMsg(stream, FrBErrors::FRB_ERR_MEMBER_NOT_FOUND, _name);
}



/*      FrBInvalidNonSharedException       */


FrBInvalidNonSharedException::FrBInvalidNonSharedException(FrBMember * f)
    : _fl(f)
{
}

std::ostream& FrBInvalidNonSharedException::put(std::ostream& stream) const
{
    frb_assert(_fl);
    return FrBErrors::putMsg(stream, FrBErrors::FRB_ERR_INVALID_NON_SHARED, _fl->fullName());
}



/*         FrBNullReferenceException       */



std::ostream& FrBNullReferenceException::put(std::ostream& stream) const
{
    return FrBErrors::putMsg(stream, FrBErrors::FRB_ERR_NULL_REF);
}



/*           FrBMeInSharedException            */

FrBMeInSharedException::FrBMeInSharedException()
{
}

std::ostream& FrBMeInSharedException::put(std::ostream& stream) const
{
    return FrBErrors::putMsg(stream, FrBErrors::FRB_ERR_ME_IN_SHARED);
}

/*            FrBInheritsSealedException               */

FrBInheritsSealedException::FrBInheritsSealedException()
{
}


std::ostream& FrBInheritsSealedException::put(std::ostream& stream) const
{
    return FrBErrors::putMsg(stream, FrBErrors::FRB_ERR_INHERITS_SEALED);
}



/*          FrBDecWithoutDefaultCtorException          */

FrBDecWithoutDefaultCtorException::FrBDecWithoutDefaultCtorException(const FrBMember * f)
    : _fl(f)
{
}
 
std::ostream& FrBDecWithoutDefaultCtorException::put(std::ostream& stream) const
{
    frb_assert(_fl);
    return FrBErrors::putMsg(stream, FrBErrors::FRB_ERR_NO_DEF_CTOR, _fl->fullName());
}


