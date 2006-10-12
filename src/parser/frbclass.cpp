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


#include "frbclass.h"
#include "frbmemory.h"
#include "frbkeywords.h"

std::ostream& operator<<(std::ostream& s, const FrBClass& c)
{
    return c.put(s);
}

/*        FrBClass            */

FrBClassMap * FrBClass::root = 0;

FrBClass * FrBClass::getClassFromString(const String& name) throw (FrBClassNotFoundException)
{
    FrBClassMap::iterator f = root->find(name);
    
    if(f == root->end())
        throw FrBClassNotFoundException(name);
    else
        return f->second;
}

void FrBClass::executeDefaultConstructor(FrBExecutionEnvironment& e, FrBBaseObject * me) const throw (FrBExecutionException)
{
    if(_defaultCtor)
    {
        _defaultCtor->execute(e, me);
    }
    else
    {
        if(_ctors.size() > 0)
            throw FrBDefaultCtorNotFoundException();
        else
            ;//TODO  default ctor creation & execution
    }
        
}

FrBBaseObject * FrBClass::createInstance(FrBExecutionEnvironment& e) const throw (FrBExecutionException)
{
    FrBBaseObject * o = allocateInstance(e);
    
    executeDefaultConstructor(e, o);
   
        
    return o;
}

FrBBaseObject * FrBClass::createInstance(FrBExecutionEnvironment& e, const FrBBaseObjectList& args) const
    throw (FrBExecutionException)
{
    FrBBaseObject * o = allocateInstance(e);
    executeConstructor(e, o, args);
    return o;
}

void FrBClass::destroyInstance(FrBExecutionEnvironment&e, FrBBaseObject * o) const throw (FrBExecutionException)
{
    executeDestructor(e, o);
    freeInstance(e, o);
}


std::ostream& FrBClass::put(std::ostream& sout, int level) const
{
    using namespace std;

    String ident(level, '\t');

    sout    << ident << "Class " << name() << " (" << specString() << ")" << endl
            << ident << "\t+ Shared=" << shared() << endl
            << ident << "\t+ Sealed=" << sealed() << endl
            << ident << "\t+ Abstract=" << abstract() << endl
            << ident << "\t+ Scope=" << scope() << endl
            << ident << "\t+ Constructors:" << endl;
            
    const ConstructorContainer * ctors = constructorList();
    
    for(ConstructorContainer::const_iterator it = ctors->begin(); it != ctors->end(); ++it)
        (*it)->put(sout, level);
            
    sout    << ident << "\t+ Destructor:" << endl;
    
    if(destructor() != 0)
        sout << ident << *destructor() << endl;
        
    sout << ident << "\t+ Functions:" << endl;
    
    const FunctionContainer * fns = functionList();
    
    
    for(FunctionContainer::const_iterator itf = fns->begin(); itf != fns->end(); ++itf)
            itf->second->put(sout, level);
    
    sout << ident << "\t+ Operators:" << endl;
    
    const OperatorContainer * ops = operatorList();
    
    
    for(OperatorContainer::const_iterator itf = ops->begin(); itf != ops->end(); ++itf)
    {
        sout << ident << "\tFor operator " << FrBKeywords::getKeywordOrSymbol(itf->first) << endl;
        itf->second->put(sout, level);
    }        
                
    sout << ident << "\t+ Inner classes:" << endl;
    
    const ClassContainer * inners = innerClassList();
    
    for(ClassContainer::const_iterator it = inners->begin(); it != inners->end(); ++it)
        it->second->put(sout, level + 1);

    return sout;
}


FrBBaseObject* FrBClass::convert(FrBBaseObject * from, const FrBClass * to) throw (FrBIncompatibleClassException)
{
    if(!areCompatibles(from->getClass(), to))
        throw FrBIncompatibleClassException(from->getClass(), to);
        
    return forceConvert(from, to);
}


FrBBaseObject* FrBClass::forceConvert(FrBBaseObject * from, const FrBClass * to)
{
    return from;
}


bool FrBClass::areCompatibles(const FrBClass * from, const FrBClass * to)
{
    return from == to;
}


FrBClass::~FrBClass()
{
    for(ClassContainer::iterator it = _innerClasses.begin(); it != _innerClasses.end(); ++it) //destuct inner classes
        delete it->second;

    _innerClasses.clear();
    
    for(FunctionContainer::iterator itf = _functions.begin(); itf != _functions.end(); ++itf) //destuct functions
        delete itf->second;

            
    _functions.clear();
        
    for(ConstructorContainer::iterator it = _ctors.begin(); it != _ctors.end(); ++it) //destuct ctors
        delete *it;
        
    _ctors.clear();
    
    delete _dtor;
};

/*        FrBCodeClass            */

FrBCodeClass::FrBCodeClass() {};

FrBCodeClass::~FrBCodeClass()
{

};

const char* FrBCodeClass::specString() const
{
    return "user code class";
}


