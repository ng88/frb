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
#include "frbexpr.h"

std::ostream& operator<<(std::ostream& s, const FrBClass& c)
{
    return c.put(s);
}

/*        FrBClass            */

void FrBClass::resolvePrototype(FrBResolveEnvironment& e) throw (FrBResolveException)
{

    //if(_resolved) return;

    if(!shared() && !hasDefaultCtor() &&  constructorList()->size() == 0)
    {
	FrBFunction * f = new FrBNopCppFunction();
	f->setName("$generated_ctor$");
	f->setSub(true);
	f->setScope(SC_PUBLIC);
	addConstructor(f);
    }

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


/*
    for(ClassContainer::iterator it = _baseClasses.begin(); it != _inClasses.end(); ++it)
    {
        frb_assert(it->second);
        it->second->resolvePrototype(e);
    }

    */
}

void FrBClass::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{

    //if(_resolved) return;
    
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

/*
    for(ClassContainer::iterator it = _baseClasses.begin(); it != _baseClasses.end(); ++it)
    {
        frb_assert(it->second);
        it->second->resolveAndCheck(e);
    }


    _resolved = true;
*/

}

/*void FrBClass::initField(FrBExecutionEnvironment& e, FrBBaseObject * o, int start_index = 0) const
  throw (FrBExecutionException)
{
  for(FieldContainer::const_iterator it = _fields.begin(); it != _fields.end(); ++it)
      if(!it->second->shared())
	  o->addField(it->second->index() + start_index, it->second->evalDefaultValue(e));
}*/

void FrBClass::initInstance(FrBExecutionEnvironment& e, FrBBaseObject * o) const
  throw (FrBExecutionException)
{

  /* put 'Me' at the top of the stack */
  e.stack().push(o);


  /* init fields of 'Me' */
  for(FieldContainer::const_iterator it = _fields.begin(); it != _fields.end(); ++it)
      if(!it->second->shared())
	  o->addField(it->second->index(), it->second->evalDefaultValue(e));

  /*int pos = 0;
  for(ClassContainer::const_iterator it = _baseClasses.begin(); it != _baseClasses.end(); ++it)
  {
      it->second->initField(e, o, pos);
      pos +=  it->second->fieldList()->size();
      }*/

  //initField(e, o, pos);
//TODO prévoir l'allocation des champs


  /* call bases constructors */
  for(ClassContainer::const_iterator it = _baseClasses.begin(); it != _baseClasses.end(); ++it)
  {
       // call def ctor, for testing
      //it->second->findConstructor()->execute(e, o);
  }


  /*

    pos <- 0
    for each base b
    {
      call initializer of class b from pos

      pos <- pos + field_count_of_b
    }

    init fields from base_count to base_count + field_count - 1
    
!!!!!!!!! index différent pour un mm champs
  */
    
  e.stack().pop();
}


FrBBaseObject * FrBClass::createInstance(FrBExecutionEnvironment& e, const FrBBaseObjectList& args) const
    throw (FrBExecutionException)
{
    FrBBaseObject * o = allocateInstance(e);
    initInstance(e, o);
    findConstructor(args)->execute(e, o, args);
    return o;
}

FrBBaseObject * FrBClass::createInstance(FrBExecutionEnvironment& e) const
  throw (FrBExecutionException)
{
    FrBBaseObject * o = allocateInstance(e);
    initInstance(e, o);
    findConstructor()->execute(e, o);
    return o;
}

FrBBaseObject * FrBClass::createInstance(FrBExecutionEnvironment& e, const FrBFunction * ctor,  const FrBBaseObjectList& args) const
    throw (FrBExecutionException)
{
    FrBBaseObject * o = allocateInstance(e);
    initInstance(e, o);
    ctor->execute(e, o, args);
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

void FrBClass::initSharedField(FrBExecutionEnvironment& e) const throw (FrBExecutionException)
{
    /* add storage space */
    e.sharedMem().addClass(this);

    /* initialize fields */
    for(FieldContainer::const_iterator it = _fields.begin(); it != _fields.end(); ++it)
	if(it->second->shared())
	{
	    FrBBaseObject * o = it->second->evalDefaultValue(e);
	    e.sharedMem().setSharedField(it->second,o );
	    std::cout << "init " << it->second->fullName() << " " << o
		      << " " << e.sharedMem().getSharedField(it->second) << "\n";

	}
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

    for(ClassContainer::const_iterator it = _baseClasses.begin(); it != _baseClasses.end(); ++it)
    {
	sout << ident << "\t Inherits " << it->second->fullName() << endl;
    }

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

    //if(to == FrBClassWrapper::getCppClass())

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

}

void FrBCodeClass::resolvePrototype(FrBResolveEnvironment& e) throw (FrBResolveException)
{

    //if(_resolved) return;

    for(URClassContainer::iterator it = _urBaseClasses.begin(); it != _urBaseClasses.end(); ++it)
    {
	(*it)->resolveAndCheck(e);
	addBaseClass((*it)->getContext());
    }


    FrBClass::resolvePrototype(e);
}

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


void FrBCodeClass::addURBaseClasse(FrBUnresolvedTypeExpr *c)
{
    frb_assert(c);

    _urBaseClasses.push_back(c);
}


const CString FrBCodeClass::specString() const
{
    return "frb interpreted class";
}

/*void FrBCodeClass::resolveAndCheck() throw (FrBResolveException)
{
}*/




