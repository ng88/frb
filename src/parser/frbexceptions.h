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


#ifndef FRBEXCEPTIONS_H
#define FRBEXCEPTIONS_H

#include "../common/string.h"
#include <vector>

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

class FrBResolveException : public FrBException
{
public:
};



class FrBEvaluationException : public FrBExecutionException
{
};




class FrBAllocationException : public FrBExecutionException
{
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




//TODO prendre en charge les candidate
class FrBFunctionNotFoundException : public FrBExecutionException, public FrBResolveException
{
protected:
    String _name;
    std::vector<const FrBClass*> _args;
    std::vector<FrBFunction*> _candidates;
    
public:
    FrBFunctionNotFoundException(const String& name, const std::vector<const FrBClass*>& args);
    FrBFunctionNotFoundException(const String& name, const std::vector<FrBBaseObject*>& args);
    FrBFunctionNotFoundException(const String& name);
    
    std::ostream& put(std::ostream& stream) const;
    
    inline const String& name() { return _name; }
    inline void addCandidate(FrBFunction* f) { _candidates.push_back(f); }
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


class FrBClassNotFoundException : public FrBExecutionException, public FrBResolveException
{
protected:
    String _name;
    
public:
    FrBClassNotFoundException(const String& name);
    std::ostream& put(std::ostream& stream) const;
    
    inline const String& name() { return _name; }
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

class FrBMemberNotFoundException : public FrBResolveException
{
protected:
    const FrBClass * _class;
    String _name;

public:
    FrBMemberNotFoundException(const FrBClass * c, const String& name);
    std::ostream& put(std::ostream& stream) const;
    
    inline const String& member() { return _name; }
    inline const FrBClass * getClass() { return _class; }
};

#endif
