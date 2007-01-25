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


#ifndef FRBEXCEPTIONS_H
#define FRBEXCEPTIONS_H

#include "../common/string.h"
#include <vector>

#include "frbexprlist.h"

class FrBMember;

//TODO TOUTES les erreurs devront être des exceptions récupérables dans l'application embarquante
//     l'interpréteur sera un cas particulier d'application embarquante (le compilo aussi)
 

class FrBClass;
class FrBBaseObject;
class FrBFunction;

class FrBException
{
public:
    virtual ~FrBException() {}
    virtual std::ostream& put(std::ostream& stream) const = 0;
};

std::ostream& operator<<(std::ostream& s, const FrBException& c);


class FrBExecutionException : public FrBException
{
public:
};

/** exception that occurs when a return statement is encountered */
class FrBReturnException : public FrBExecutionException
{
private:
  FrBBaseObject * _val;

public:
  inline FrBReturnException(FrBBaseObject *v = 0) : _val(v) {}
  inline FrBBaseObject* value() { return _val; }

  std::ostream& put(std::ostream& stream) const { return stream; }

};

class FrBResolveException : public FrBException
{
public:
};



class FrBEvaluationException : public FrBExecutionException
{
};


class FrBInvalidLValueException : public FrBExecutionException
{
protected:
    String _lvalue;
    
public:
     FrBInvalidLValueException(const String& lvalue);
     FrBInvalidLValueException(const FrBExpr* lvalue);
     std::ostream& put(std::ostream& stream) const;
};




class FrBAllocationException : public FrBExecutionException
{
};


class FrBStackOverflowException : public FrBExecutionException
{
public:

    std::ostream& put(std::ostream& stream) const;

};


class FrBUndeclaredIdentifierException : public FrBExecutionException
{
protected:
    String _name;
    
public:
    FrBUndeclaredIdentifierException(const String& name);
    std::ostream& put(std::ostream& stream) const;
    
    inline const String& name() { return _name; }
};


class FrBMemberNotFoundException : public FrBExecutionException, public FrBResolveException
{
protected:
    String _name;

public:
    FrBMemberNotFoundException(const String& name);

    inline const String& name() { return _name; }
    
    std::ostream& put(std::ostream& stream) const;

};

class FrBFieldNotFoundException : public FrBMemberNotFoundException
{
    
public:
    FrBFieldNotFoundException(const String& name);
    
    std::ostream& put(std::ostream& stream) const;

};

//TODO prendre en charge les candidate
class FrBFunctionNotFoundException : public FrBMemberNotFoundException
{
protected:
    std::vector<const FrBClass*> _args;
    std::vector<FrBFunction*> _candidates;
    
public:
    FrBFunctionNotFoundException(const String& name, const std::vector<const FrBClass*>& args);
    FrBFunctionNotFoundException(const String& name, const std::vector<FrBBaseObject*>& args);
    FrBFunctionNotFoundException(const String& name, const FrBExprList& args);
    FrBFunctionNotFoundException(const String& name);
    
    std::ostream& put(std::ostream& stream) const;

    inline void addCandidate(FrBFunction* f) { _candidates.push_back(f); }
};

//TODO to be improved
class FrBFunctionAmbiguityException : public FrBFunctionNotFoundException
{
public:

    FrBFunctionAmbiguityException(const String& name);
    std::ostream& put(std::ostream& stream) const;
};

class FrBDefaultCtorNotFoundException : public FrBFunctionNotFoundException
{
public:
    FrBDefaultCtorNotFoundException();
};



class FrBDtorNotFoundException : public FrBFunctionNotFoundException
{
public:
    FrBDtorNotFoundException();
};


class FrBClassNotFoundException : public FrBMemberNotFoundException
{

    
public:
    FrBClassNotFoundException(const String& name);
    std::ostream& put(std::ostream& stream) const;

};


class FrBIncompatibleClassException : public FrBExecutionException, public FrBResolveException
{
private:
    const FrBClass * _provided;
    const FrBClass * _needed;
    
public:
    FrBIncompatibleClassException(const FrBClass * provided, const FrBClass * needed);
    std::ostream& put(std::ostream& stream) const;
};



/** Occurs when using a non static member as a static member  */
class FrBInvalidNonSharedException : public FrBExecutionException
{
protected:
    FrBMember *  _fl;
public:

    FrBInvalidNonSharedException(FrBMember * f);
    std::ostream& put(std::ostream& stream) const;

};

class FrBNullReferenceException : public FrBExecutionException
{
protected:

public:

    std::ostream& put(std::ostream& stream) const;

};


class FrBMeInSharedException : public FrBResolveException
{
protected:


public:
    FrBMeInSharedException();

    std::ostream& put(std::ostream& stream) const;

};


#endif
