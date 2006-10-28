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


#include "frbexceptions.h"
#include "frbkeywords.h"
#include "frberrors.h"
#include "frbbaseobject.h"
#include "frbclass.h"
#include "frbexpr.h"

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

/*             FrBFunctionNotFoundException                */


FrBFunctionNotFoundException::FrBFunctionNotFoundException(const String& name)
    : _name(name)
{
}

FrBFunctionNotFoundException::FrBFunctionNotFoundException(const String& name, const std::vector<const FrBClass*>& args)
    : _name(name), _args(args)
{
}

FrBFunctionNotFoundException::FrBFunctionNotFoundException(const String& name, const std::vector<FrBBaseObject*>& args)
    : _name(name)
{
    for(std::vector<FrBBaseObject*>::const_iterator it = args.begin(); it != args.end(); ++it)
        _args.push_back( (*it)->getClass() );
}

FrBFunctionNotFoundException::FrBFunctionNotFoundException(const String& name, const FrBExprList& args)
{
    for(FrBExprList::const_iterator it = args.begin(); it != args.end(); ++it)
        _args.push_back( (*it)->getClass() );
}



std::ostream& FrBFunctionNotFoundException::put(std::ostream& stream) const
{
    return stream << _name << ": " << FrBErrors::getMsg(FrBErrors::FRB_ERR_FN_NOT_FOUND);
}


/*                   FrBDefaultCtorNotFoundException                */


FrBDefaultCtorNotFoundException::FrBDefaultCtorNotFoundException()
 : FrBFunctionNotFoundException(FrBKeywords::getKeyword(FrBKeywords::FRB_KW_CONSTRUCTOR_NAME))
{
}

/*                   FrBDtorNotFoundException                */


FrBDtorNotFoundException::FrBDtorNotFoundException()
 : FrBFunctionNotFoundException(FrBKeywords::getKeyword(FrBKeywords::FRB_KW_DESTRUCTOR_NAME))
{

}


/*             FrBClassNotFoundException             */

FrBClassNotFoundException::FrBClassNotFoundException(const String& name)
    : _name(name)
{
}

std::ostream& FrBClassNotFoundException::put(std::ostream& stream) const
{
    return stream << _name << ": " << FrBErrors::getMsg(FrBErrors::FRB_ERR_CLASS_NOT_FOUND);
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


FrBMemberNotFoundException::FrBMemberNotFoundException(const FrBClass * c, const String& name)
 : _class(c), _name(name)
{
}


std::ostream& FrBMemberNotFoundException::put(std::ostream& stream) const
{
    return FrBErrors::putMsg(stream, FrBErrors::FRB_ERR_MB_NOT_FOUND, _name, _class->name());
}


