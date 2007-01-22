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


#include "frbclass.h"
#include "frbmemory.h"
#include "frbkeywords.h"
#include "../common/assert.h"
#include "frbbuiltinclasses.h"


std::ostream& operator<<(std::ostream& s, const FrBClass& c)
{
    return c.put(s);
}

/*        FrBClass            */

void FrBClass::resolvePrototype(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    for(ConstructorContainer::iterator it = _ctors.begin(); it != _ctors.end(); ++it)
    {
        frb_assert((*it));
        (*it)->resolvePrototype(e);
    }
    
    if(_dtor) _dtor->resolvePrototype(e);

    for(FieldContainer::iterator it = _fields.begin(); it != _fields.end(); ++it)
    {
        frb_assert(it->second);
        it->second->resolvePrototype(e);
    }


    for(FunctionContainer::iterator itf = _functions.begin(); itf != _functions.end(); ++itf)
    {
        frb_assert(itf->second);
        itf->second->resolvePrototype(e);
    }
    
    for(OperatorContainer::iterator itf = _operators.begin(); itf != _operators.end(); ++itf)
    {
        frb_assert(itf->second);
        itf->second->resolvePrototype(e);
    }
    
    for(ClassContainer::iterator it = _innerClasses.begin(); it != _innerClasses.end(); ++it)
    {
        frb_assert(it->second);
        it->second->resolvePrototype(e);
    }
}

void FrBClass::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    
    for(ConstructorContainer::iterator it = _ctors.begin(); it != _ctors.end(); ++it)
    {
        frb_assert((*it));
        (*it)->resolveAndCheck(e);
    }
    
    if(_dtor) _dtor->resolveAndCheck(e);

    for(FieldContainer::iterator it = _fields.begin(); it != _fields.end(); ++it)
    {
        frb_assert(it->second);
        it->second->resolveAndCheck(e);
    }

    for(FunctionContainer::iterator itf = _functions.begin(); itf != _functions.end(); ++itf)
    {
        frb_assert(itf->second);
        itf->second->resolveAndCheck(e);
    }
    
    for(OperatorContainer::iterator itf = _operators.begin(); itf != _operators.end(); ++itf)
    {
        frb_assert(itf->second);
        itf->second->resolveAndCheck(e);
    }
    
    for(ClassContainer::iterator it = _innerClasses.begin(); it != _innerClasses.end(); ++it)
    {
        frb_assert(it->second);
        it->second->resolveAndCheck(e);
    }
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

void FrBClass::initInstance(FrBExecutionEnvironment& e, FrBBaseObject * o) const
  throw (FrBExecutionException)
{
  e.stack().push(o);
  
  int i = 0;
  for(FieldContainer::const_iterator it = _fields.begin(); it != _fields.end(); ++it)
      o->addField(i++, it->second->evalDefaultValue(e));
    
  e.stack().pop();
}

FrBBaseObject * FrBClass::createInstance(FrBExecutionEnvironment& e) const
  throw (FrBExecutionException)
{
    FrBBaseObject * o = allocateInstance(e);
    initInstance(e, o);
    executeDefaultConstructor(e, o);
    return o;
}

FrBBaseObject * FrBClass::createInstance(FrBExecutionEnvironment& e, const FrBBaseObjectList& args) const
    throw (FrBExecutionException)
{
    FrBBaseObject * o = allocateInstance(e);
    initInstance(e, o);
    executeConstructor(e, o, args);
    return o;
}

void FrBClass::destroyInstance(FrBExecutionEnvironment& e, FrBBaseObject * o) const throw (FrBExecutionException)
{
    /** if o is managed by the GC */
    if(o->isManaged())
        e.memory()->removeObjectFromGC(o);

     //TODO executeDestructor(e, o);
    freeInstance(e, o);

}


std::ostream& FrBClass::put(std::ostream& sout, int level) const
{
    using namespace std;

    String ident(level, '\t');

    level++;

    sout    << ident 
	    << FrBKeywords::scopeToString(scope()) << ' '
	    << FrBKeywords::sharedToString(shared()) << ' '
	    << FrBKeywords::sealedToString(sealed()) << ' '
	    << FrBKeywords::abstractToString(abstract()) << ' '
	    << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_CLASS) << ' '
	    << name() << "\t' " << specString() << endl;

    for(FieldContainer::const_iterator it = _fields.begin(); it != _fields.end(); ++it)
    {
      sout << ident << '\t';
      it->second->put(sout, level);
    }

    const ConstructorContainer * ctors = constructorList();
    
    for(ConstructorContainer::const_iterator it = ctors->begin(); it != ctors->end(); ++it)
    {
      sout << ident << "\t'As constructor " << endl;
      (*it)->put(sout, level);
    }
                
    if(destructor())
    {
      sout << ident << "\t'As destructor "<< endl;
      destructor()->put(sout, level);
    }
        
    const FunctionContainer * fns = functionList();
    
    for(FunctionContainer::const_iterator itf = fns->begin(); itf != fns->end(); ++itf)
            itf->second->put(sout, level);
    
    const OperatorContainer * ops = operatorList();
        
    for(OperatorContainer::const_iterator itf = ops->begin(); itf != ops->end(); ++itf)
    {
        sout << ident << "\t'As operator " << FrBKeywords::getKeywordOrSymbol(itf->first) << endl;
        itf->second->put(sout, level);
    }        
    
    const ClassContainer * inners = innerClassList();
    
    for(ClassContainer::const_iterator it = inners->begin(); it != inners->end(); ++it)
        it->second->put(sout, level);

    return sout << ident
		<< FrBKeywords::getKeyword(FrBKeywords::FRB_KW_END) << ' '
		<< FrBKeywords::getKeyword(FrBKeywords::FRB_KW_CLASS) << endl;
}


FrBBaseObject* FrBClass::convert(FrBBaseObject * from, const FrBClass * to) throw (FrBIncompatibleClassException)
{
    if(!from->getClass()->isCompatibleWith(to))
        throw FrBIncompatibleClassException(from->getClass(), to);
        
    return forceConvert(from, to);
}


FrBBaseObject* FrBClass::forceConvert(FrBBaseObject * from, const FrBClass * to)
{
    return from;
}


bool FrBClass::isCompatibleWith(const FrBClass * to) const
{
    frb_assert2(to, "null pointer passed to FrBClass::isCompatibleWith()");
    //cas particulier si on a une classe à gauche
    //void (ie sub return) can not be converted or used in expression
    if(this == FrBVoid::getCppClass())
        return false;
    
    //null can be used with all type
    if(this == FrBNull::getCppClass())
        return true;

    return to == FrBObject::getCppClass() || this == to;
}

bool FrBClass::inheritsFrom(const FrBClass * from) const
{
    //TODO a faire
    return isCompatibleWith(from);
}


FrBMember* FrBClass::getMember(const String& name) throw (FrBMemberNotFoundException)
{
    /* look for a field first */
    
    try
    {
        return findField(name);
    }
    catch(FrBFieldNotFoundException ex)
    {
        /* look for a function */
        
        FnPairIt pit = findFunctions(name);
        
        if(pit.second->second)
            throw FrBFunctionAmbiguityException(name);
        else if(pit.first != functionList()->end())
            return pit.first->second;
        else
            throw FrBFunctionNotFoundException(name);
    }

}


FrBClass::~FrBClass()
{

    for(ClassContainer::iterator it = _innerClasses.begin(); it != _innerClasses.end(); ++it) //destuct inner classes
        delete it->second;

    _innerClasses.clear();

    for(FunctionContainer::iterator itf = _functions.begin(); itf != _functions.end(); ++itf) //destuct functions
        delete itf->second;

    _functions.clear();

    for(OperatorContainer::iterator itf = _operators.begin(); itf != _operators.end(); ++itf) //destuct operators
        delete itf->second;

    _operators.clear();

    for(FieldContainer::iterator itf = _fields.begin(); itf != _fields.end(); ++itf) //destuct fields
        delete itf->second;

    _fields.clear();

    for(ConstructorContainer::iterator it = _ctors.begin(); it != _ctors.end(); ++it) //destuct ctors
        delete *it;

    _ctors.clear();

    if(_dtor)
        delete _dtor; //destruct dtor

};

/*        FrBCodeClass            */

FrBCodeClass::FrBCodeClass() {};

FrBCodeClass::~FrBCodeClass()
{

};

FrBBaseObject * FrBCodeClass::allocateInstance(FrBExecutionEnvironment& e) const
  throw (FrBAllocationException)
{
  FrBBaseObject * o = new FrBUserObject(this, _fields.size());
  e.addGarbagedObject(o);
  return o;
}
    
void FrBCodeClass::freeInstance(FrBExecutionEnvironment&, FrBBaseObject * o) const
  throw (FrBAllocationException)
{
  delete o;
}


const CString FrBCodeClass::specString() const
{
    return "frb interpreted class";
}

/*void FrBCodeClass::resolveAndCheck() throw (FrBResolveException)
{
}*/




