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


#ifndef FRBCLASS_H
#define FRBCLASS_H

#include <map>
#include <vector>
#include <iostream>
#include "../common/assert.h"
#include "../common/string.h"
#include "../common/iterators.h"
#include "frbbaseobject.h"
#include "frbfunction.h"
#include "frbfield.h"
#include "frbkeywords.h"

typedef char op_t;


//utiliser une multimap pour les fonctions

class FrBClass;


std::ostream& operator<<(std::ostream& s, const FrBClass& c);


//TODO: utiliser une hash_map à la place
typedef std::map<const String, FrBFunction*> FrBFunctionMap;

class FrBClass : public FrBMember /* a frb class in memory */
{
public:

    typedef std::multimap<const String, FrBFunction*>   FunctionContainer;
    typedef std::map<const String, FrBField*>           FieldContainer;
    typedef std::multimap<int, FrBFunction*>            OperatorContainer;
    typedef FrBFunctionList                             ConstructorContainer;
    typedef FrBClassMap                                 ClassContainer;
    typedef FunctionContainer::const_iterator           FnContIt;
    typedef std::pair<FnContIt, FnContIt>               FnPairIt;

private:

    /** init stuff for o (create fields, ...*/
    void initInstance(FrBExecutionEnvironment& e, FrBBaseObject * o) const
      throw (FrBExecutionException);
    
protected:



    bool     _shared; /* shared class, ie module */
    bool     _abstract; /* abstract class, ie interface */
    bool     _sealed; /* sealed class */

    scope_t  _scope;
    String   _name;

    FunctionContainer _functions;
    ClassContainer _innerClasses;

    FieldContainer _fields;

    OperatorContainer _operators;
    
    ConstructorContainer _ctors;
    FrBFunction * _defaultCtor;
    FrBFunction * _dtor;
    
    /** Create a new non-initialized instance
      * when redefining allocateInstance() you can choose to use the GC or not
      * in the both case you MUST redefine freeInstance()
      * Note: call e.addGarbagedObject(your_object) to use the GC
      */
    virtual FrBBaseObject * allocateInstance(FrBExecutionEnvironment& e) const throw (FrBAllocationException) = 0;

    /** Free instance without call the destructor for object that are not managed by the GC */
    virtual void freeInstance(FrBExecutionEnvironment& e, FrBBaseObject * o) const
        throw (FrBAllocationException) = 0;
    
public:

    inline FrBClass();
    virtual ~FrBClass();
    
        //TODO:  fn_member_list; ...
             /*fn_member_list;
             data_member_list;
             property_list;
             class_list;
             implements_list; contenu dans base_list
             inherits_list;
             
             callFunction/propety/getMember <- virtual = 0 ici
             
             virtualité partout, trouver une syntaxe plus sympa pour la virtualité pure
             */
    /** Used in type resolution */
    virtual void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    virtual void resolvePrototype(FrBResolveEnvironment&) throw (FrBResolveException);
    
    template<class ArgContainer>
    inline FrBFunction * findConstructor(const ArgContainer& args) const
      throw (FrBFunctionNotFoundException);
    
    inline FnPairIt findFunctions(const String& name) const;
    
    template<class ArgContainer>
    inline FrBFunction * findFunction(const String& name, const ArgContainer& args) const
      throw (FrBFunctionNotFoundException);

    template<class ArgContainer>
    inline FrBFunction * findOperator(int op, const ArgContainer& args) const
      throw (FrBFunctionNotFoundException);

    inline FrBFunction * findDestructor() const
      throw (FrBFunctionNotFoundException);

    inline FrBField * findField(const String& name) const
      throw (FrBFieldNotFoundException);
    
    /** Execute the default constructor (with no param). If no ctor exists and if there are no other ctor, a default ctor is created and executed */
    void executeDefaultConstructor(FrBExecutionEnvironment&, FrBBaseObject * me) const
        throw (FrBExecutionException);
    
    inline void executeConstructor(FrBExecutionEnvironment& e, FrBBaseObject * me,
	   const FrBBaseObjectList& args) const throw (FrBExecutionException);
    
    inline FrBBaseObject * executeFunction(FrBExecutionEnvironment& e,
					   const String& name,
					   FrBBaseObject * me,
					   const FrBBaseObjectList& args) const
                throw (FrBExecutionException);
    
    inline FrBBaseObject * executeOperator(FrBExecutionEnvironment& e,
					   int op,
					   FrBBaseObject * me,
					   const FrBBaseObjectList& args) const
               throw (FrBExecutionException);
    
    inline FrBBaseObject * executeDestructor(FrBExecutionEnvironment& e, FrBBaseObject * me) const
               throw (FrBExecutionException);

    /*virtual FrBBaseObject * callOperator(String name, FrBBaseObjectList args) = 0;
    virtual FrBBaseObject * getProperty(String name, FrBBaseObjectList args) = 0;
    virtual FrBBaseObject * setProperty(String name, FrBBaseObjectList args) = 0;// faire une binaryOperator pr optimiser
    virtual FrBBaseObject * getMember(String name) = 0;*/
    
    //TODO /* throw si existe deja */
    inline void addInnerClass(FrBClass * c);
    inline void addField(FrBField * c);
    inline void addFunction(FrBFunction * f);
    //TODO vérifier que au moins l'un des params concerne la classe (ie pour int il faut 
    // op(int, int) op(int, double) ou op(double, int) mais pas op(double, single)
    inline void addOperator(int op, FrBFunction * f);
    //TODO /* verifier la validité des params et retour */
    inline void addConstructor(FrBFunction * f);
    inline void addDestructor(FrBFunction * f);

    inline const ClassContainer* innerClassList() const { return &_innerClasses; }
    inline const OperatorContainer* operatorList() const { return &_operators; }
    inline const FunctionContainer* functionList() const { return &_functions; }
    inline const ConstructorContainer* constructorList() const { return &_ctors; }
    inline const FrBFunction* destructor() const { return _dtor; }
    
    inline ClassContainer* innerClassPtr() { return &_innerClasses; }
    
    
    inline bool shared() const { return _shared; }
    inline bool abstract() const { return _abstract; }
    inline bool sealed() const { return _sealed; }
    inline scope_t scope() const { return _scope; }
    inline String name() const { return _name; }
    
    inline void setShared(bool v) { _shared = v; }
    inline void setAbstract(bool v) { _abstract = v; }
    inline void setSealed(bool v) { _sealed = v; }
    inline void setScope(scope_t v) { _scope = v; }
    inline void setName(String v) { _name = v; }
    
    inline unsigned int typeID() const { return reinterpret_cast<unsigned int>(this); }
   
    String fullName() const;
    
    /** allocate instance and call the appropriate constructor */
    FrBBaseObject * createInstance(FrBExecutionEnvironment& e) const throw (FrBExecutionException);
    FrBBaseObject * createInstance(FrBExecutionEnvironment&e, const FrBBaseObjectList& args) const
        throw (FrBExecutionException);
    
    /** call the destructor and free instance (call freeInstance to free the object)*/
    void destroyInstance(FrBExecutionEnvironment& e, FrBBaseObject * o) const throw (FrBExecutionException);
    
    virtual std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    /** String describing this overload of class */
    virtual const char* specString() const = 0;
    
    /** Check if from and to are compatibles and convert from to to */
    static FrBBaseObject* convert(FrBBaseObject * from, const FrBClass * to)throw (FrBIncompatibleClassException);
    
    /** Convert from from to to without check */
    static FrBBaseObject* forceConvert(FrBBaseObject * from, const FrBClass * to);
    
    
    /** Check if this and to are compatibles (/!\ a->isCompatibleWith(b) != b->isCompatibleWith(a)) */
    bool isCompatibleWith(const FrBClass * to) const;
    
    /** Return true if current class inherits from from */
    bool inheritsFrom(const FrBClass * from) const;
    
    /** can resolve any type of member (function, inner class, field, ...) */
    FrBMember* getMember(const String& name) throw (FrBMemberNotFoundException);

    
};



/** FrB class from FrB code */
class FrBCodeClass : public FrBClass
{

protected:

  FrBBaseObject * allocateInstance(FrBExecutionEnvironment& e) const
    throw (FrBAllocationException);

  void freeInstance(FrBExecutionEnvironment&, FrBBaseObject * o) const
    throw (FrBAllocationException);

public:
    FrBCodeClass();
    ~FrBCodeClass();


    
    virtual const char* specString() const;
    //void resolveAndCheck() throw (FrBResolveException);
   
};


typedef std::stack<FrBCodeClass*> FrBCodeClassStack;



































/*          Inlined         */


inline FrBClass::FrBClass()
  : _shared(false), _abstract(false), 
    _sealed(false), _scope(SC_PRIVATE),_defaultCtor(0), _dtor(0)
{
}
    
template<class ArgContainer>
inline FrBFunction * FrBClass::findConstructor(const ArgContainer& args) const
     throw (FrBFunctionNotFoundException)
{
  return FrBFunction::findOverload(FrBKeywords::getKeyword(FrBKeywords::FRB_KW_CONSTRUCTOR_NAME),
                                       _ctors.begin(), _ctors.end(), args);
}
    
inline FrBClass::FnPairIt FrBClass::findFunctions(const String& name) const
{
  return _functions.equal_range(name);
}
    
template<class ArgContainer>
inline FrBFunction * FrBClass::findFunction(const String& name, const ArgContainer& args) const
     throw (FrBFunctionNotFoundException)
{
  FnPairIt seq = _functions.equal_range(name);
        
  return FrBFunction::findOverload(name,
				   const_map_snd_iterator<FunctionContainer>(seq.first),
				   const_map_snd_iterator<FunctionContainer>(seq.second),
				   args);
}

template<class ArgContainer>
inline FrBFunction * FrBClass::findOperator(int op, const ArgContainer& args) const
     throw (FrBFunctionNotFoundException)
{
  typedef OperatorContainer::const_iterator It;
  std::pair<It, It> seq = _operators.equal_range(op);
        
  return FrBFunction::findOverload(FrBKeywords::getKeyword(FrBKeywords::FRB_KW_OPERATOR) + ((char)op),
				   const_map_snd_iterator<OperatorContainer>(seq.first),
				   const_map_snd_iterator<OperatorContainer>(seq.second),
				   args);
}

    
inline FrBFunction * FrBClass::findDestructor() const
     throw (FrBFunctionNotFoundException) 
{
  if(_dtor == 0)
    throw FrBDtorNotFoundException();
            
  return _dtor;
}

inline FrBField * FrBClass::findField(const String& name) const
     throw (FrBFieldNotFoundException)
{
    FieldContainer::const_iterator f = _fields.find(name);
    
    if(f == _fields.end())
        throw FrBFieldNotFoundException(name);
    else
    {
        frb_assert(f->second);
        return f->second;
    }
}
   
inline void FrBClass::executeConstructor(FrBExecutionEnvironment& e, FrBBaseObject * me,
			       const FrBBaseObjectList& args) const
     throw (FrBExecutionException) 
{
  findConstructor(args)->execute(e, me, args);
}
    
inline FrBBaseObject * FrBClass::executeFunction(FrBExecutionEnvironment& e, const String& name, FrBBaseObject * me,
				       const FrBBaseObjectList& args) const
     throw (FrBExecutionException) 
{
  return findFunction(name, args)->execute(e, me, args);
}
    
inline FrBBaseObject * FrBClass::executeOperator(FrBExecutionEnvironment& e, int op, FrBBaseObject * me,
				       const FrBBaseObjectList& args) const
     throw (FrBExecutionException) 
{
  return findOperator(op, args)->execute(e, me, args);
}
    
inline FrBBaseObject * FrBClass::executeDestructor(FrBExecutionEnvironment& e, FrBBaseObject * me) const
     throw (FrBExecutionException)
{
  FrBBaseObjectList args;
  return findDestructor()->execute(e, me, args);
}


inline void FrBClass::addInnerClass(FrBClass * c)
{
  frb_assert(c);

  c->setContainer(this);
  _innerClasses[c->name()] = c;
}

inline void FrBClass::addField(FrBField * c)
{
  frb_assert(c);

  c->setContainer(this);
  _fields[c->name()] = c;
}

inline void FrBClass::addFunction(FrBFunction * f)
{
  frb_assert(f);
  f->setContainer(this);
  _functions.insert(std::make_pair(f->name(), f)); 
}

inline void FrBClass::addOperator(int op, FrBFunction * f)
{
  frb_assert(f);
  f->setContainer(this);
  _operators.insert(std::make_pair(op, f));
}

inline void FrBClass::addConstructor(FrBFunction * f)
{
  frb_assert2(f, "FrBClass::addConstructor(FrBFunction f) -- f is null");

  f->setContainer(this); 

  if(f->parameterCount() == 0)
    _defaultCtor = f;
            
  _ctors.push_back(f);
}

inline void FrBClass::addDestructor(FrBFunction * f) 
{
  frb_assert(f);
  f->setContainer(this);
  _dtor = f;
}

#endif

