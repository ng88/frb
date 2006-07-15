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

typedef char op_t;


//utiliser une multimap pour les fonctions

class FrBClass;
class FrBCodeFunction;

std::ostream& operator<<(std::ostream& s, const FrBClass& c);


//TODO: utiliser une hash_map à la place
typedef std::map<const String, FrBFunction*> FrBFunctionMap;

// FrBConstClass throws des exceptions COnstException en cas d'apples de modificateurs

class FrBClass /* a frb class in memory */
{
public:

    typedef std::multimap<const String, FrBFunction*>   FunctionContainer;
    typedef std::multimap<int, FrBFunction*>            OperatorContainer;
    typedef FrBFunctionList                             ConstructorContainer;
    typedef FrBClassMap                                 ClassContainer;
    

    virtual ~FrBClass();
    

    virtual FrBFunction * findConstructor(const FrBBaseObjectList& args) const
          throw (FrBFunctionNotFoundException) = 0;
    
    virtual FrBFunction * findFunction(const String& name, const FrBBaseObjectList& args) const
         throw (FrBFunctionNotFoundException) = 0;
    
    virtual FrBFunction * findOperator(int op, const FrBBaseObjectList& args) const
         throw (FrBFunctionNotFoundException) = 0;
    
    virtual FrBFunction * findDestructor() const
        throw (FrBFunctionNotFoundException) = 0;
    
    
    virtual void executeDefaultConstructor(FrBBaseObject * me) const throw (FrBExecutionException) = 0;
    
    inline void executeConstructor(FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException) 
    {
        findConstructor(args)->execute(me, args);
    }
    
    inline FrBBaseObject * executeFunction(const String& name, FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException) 
    {
        return findFunction(name, args)->execute(me, args);
    }
    
    inline FrBBaseObject * executeOperator(int op, FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException) 
    {
        return findOperator(op, args)->execute(me, args);
    }
    
    inline FrBBaseObject * executeDestructor(FrBBaseObject * me) const
        throw (FrBExecutionException)
    {
        FrBBaseObjectList args;
        return findDestructor()->execute(me, args);
    }
        
    virtual void addInnerClass(FrBClass * c) = 0;
    virtual void addFunction(FrBFunction * f) = 0;
    virtual void addConstructor(FrBFunction * f) = 0;
    virtual void addDestructor(FrBFunction * f) = 0;

    virtual const ClassContainer* innerClassList() const  = 0;
    virtual const FunctionContainer* functionList() const  = 0;
    virtual const ConstructorContainer* constructorList() const  = 0;
    virtual const FrBFunction* destructor() const  = 0;
    
    
    virtual bool shared() const  = 0;
    virtual bool abstract() const  = 0;
    virtual bool sealed() const  = 0;
    virtual scope_t scope() const  = 0;
    virtual String name() const  = 0;
    
    virtual void setShared(bool v)  = 0;
    virtual void setAbstract(bool v)  = 0;
    virtual void setSealed(bool v)  = 0;
    virtual void setScope(scope_t v)  = 0;
    virtual void setName(String v)  = 0;
    
    virtual unsigned int typeID() const { return reinterpret_cast<unsigned int>(this); }
    
    /** Create a new non-initialized instance */
    virtual FrBBaseObject * allocateInstance() const throw (FrBAllocationException) = 0;
    
    /** sur place aussi, appel de allocateInstance() et du constructeur */
    FrBBaseObject * createInstance() const throw (FrBExecutionException);
    FrBBaseObject * createInstance(const FrBBaseObjectList& args) const throw (FrBExecutionException);
    void destroyInstance(FrBBaseObject * o) const throw (FrBExecutionException);
    
    virtual std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    /** String describing this overload of class */
    virtual const char* specString() const = 0;
    
    
    static FrBClassMap * root;
    static FrBClass * getClassFromString(const String& name) throw (FrBClassNotFoundException);
    
    static FrBBaseObject* convert(FrBBaseObject * from, const FrBClass * to) throw (FrBIncompatibleClassException)
    {
        if(from->getClass() != to)
            throw FrBIncompatibleClassException(from->getClass(), to);
            
        return from;
    }
    
};


//TODO seules les fonctions const peuvent êtres exécutés etc...

/** FrBConstClass makes a FrBClass const */
class FrBConstClass : public FrBClass
{
private:

    FrBClass * _class;
    
public:

    FrBConstClass(FrBClass * c) : _class(c) {}

    ~FrBConstClass() {}
    

    FrBFunction * findConstructor(const FrBBaseObjectList& args) const
          throw (FrBFunctionNotFoundException) { return _class->findConstructor(args); }
    
    FrBFunction * findFunction(const String& name, const FrBBaseObjectList& args) const
         throw (FrBFunctionNotFoundException) { return _class->findFunction(name, args); }
    
    FrBFunction * findOperator(int op, const FrBBaseObjectList& args) const
         throw (FrBFunctionNotFoundException) { return _class->findOperator(op, args); }
    
    FrBFunction * findDestructor() const
        throw (FrBFunctionNotFoundException) { return _class->findDestructor(); }
    
    
    void executeDefaultConstructor(FrBBaseObject * me) const
        throw (FrBExecutionException) { _class->executeDefaultConstructor(me); }
    
        
    void addInnerClass(FrBClass * c) { _class->addInnerClass(c); }
    void addFunction(FrBFunction * f) { _class->addFunction(f); }
    void addConstructor(FrBFunction * f) { _class->addConstructor(f); }
    void addDestructor(FrBFunction * f) { _class->addDestructor(f); }

    const ClassContainer* innerClassList() const { return _class->innerClassList(); }
    const FunctionContainer* functionList() const { return _class->functionList(); }
    const ConstructorContainer* constructorList() const { return _class->constructorList(); }
    const FrBFunction* destructor() const { return _class->destructor(); }
    
    
    bool shared() const  { return _class->shared(); }
    bool abstract() const { return _class->abstract(); }
    bool sealed() const { return _class->sealed(); }
    scope_t scope() const  { return _class->scope(); }
    String name() const  { return _class->name(); }
    
    void setShared(bool v) { _class->setShared(v); }
    void setAbstract(bool v) { _class->setAbstract(v); }
    void setSealed(bool v) { _class->setSealed(v); }
    void setScope(scope_t v) { _class->setScope(v); }
    void setName(String v) { _class->setName(v); }
    
    unsigned int typeID() const { return reinterpret_cast<unsigned int>(_class); }
    
    /** Create a new non-initialized instance */
    FrBBaseObject * allocateInstance() const
        throw (FrBAllocationException) { return _class->allocateInstance(); }
    
    std::ostream& put(std::ostream& stream, int indent = 0) const { return _class->put(stream, indent); }
    
    /** String describing this overload of class */
    const char* specString() const { return _class->specString(); }

    
};


class FrBImplClass : public FrBClass
{
protected:



    bool     _shared; /* shared class, ie module */
    bool     _abstract; /* abstract class, ie interface */
    bool     _sealed; /* sealed class */
    
    //TODO: utiliser des maps partout
    
    scope_t  _scope;
    String   _name;

    FunctionContainer _functions;
    ClassContainer _innerClasses;
    
    OperatorContainer _operators; /* optimiser avec un int */
    
    ConstructorContainer _ctors;
    FrBFunction * _defaultCtor;
    FrBFunction * _dtor;

public:

    FrBImplClass()
     : _shared(false), _abstract(false), 
       _sealed(false), _scope(SC_PRIVATE),_defaultCtor(0), _dtor(0) {}
    
    ~FrBImplClass();
    
        //TODO:  fn_member_list; ...
             /*fn_member_list;
             data_member_list;
             property_list;
             class_list;
             implements_list; contenu dans base_list
             inherits_list;
             
             callFunction/propety/getMember <- virtual = 0 ici
             
             virtualité partout, trouvé une synatxe plus sympa pour la virtualité pure
             */
             
             /* get function get operator --> execute,tout ca se fait sur place */
             
             /* a oter tout ca */

//TODO mettre la def des inline en dessous avec le mot cleef inline, pas de mot clef inline en déclaration
    
    FrBFunction * findConstructor(const FrBBaseObjectList& args) const
          throw (FrBFunctionNotFoundException)
    {
        return FrBFunction::findOverload(_ctors.begin(), _ctors.end(), args);
    }
    
    FrBFunction * findFunction(const String& name, const FrBBaseObjectList& args) const
         throw (FrBFunctionNotFoundException)
    {
        typedef FunctionContainer::const_iterator It;
        std::pair<It, It> seq = _functions.equal_range(name);
        
        return FrBFunction::findOverload(
                const_map_snd_iterator<FunctionContainer>(seq.first),
                const_map_snd_iterator<FunctionContainer>(seq.second),
                args);
    }
    
    FrBFunction * findOperator(int op, const FrBBaseObjectList& args) const
         throw (FrBFunctionNotFoundException)
    {
        typedef OperatorContainer::const_iterator It;
        std::pair<It, It> seq = _operators.equal_range(op);
        
        return FrBFunction::findOverload(
                const_map_snd_iterator<OperatorContainer>(seq.first),
                const_map_snd_iterator<OperatorContainer>(seq.second),
                args);
    }
    
    FrBFunction * findDestructor() const
        throw (FrBFunctionNotFoundException) 
    {
        if(_dtor == 0)
            throw FrBDtorNotFoundException();
            
        return _dtor;
    }
    
    /** Execute the default constructor (with no param). If no ctor exists and if there are no other ctor, a default ctor is created and executed */
    void executeDefaultConstructor(FrBBaseObject * me) const throw (FrBExecutionException);
        
    /*virtual FrBBaseObject * callOperator(String name, FrBBaseObjectList args) = 0;
    virtual FrBBaseObject * getProperty(String name, FrBBaseObjectList args) = 0;
    virtual FrBBaseObject * setProperty(String name, FrBBaseObjectList args) = 0;// faire une binaryOperator pr optimiser
    virtual FrBBaseObject * getMember(String name) = 0;*/
    
    //TODO /* throw si existe deja */
    void addInnerClass(FrBClass * c) { _innerClasses[c->name()] = c; }
    void addFunction(FrBFunction * f) { _functions.insert(std::make_pair(f->name(), f)); }
    //TODO /* verifier la validité des params et retour */
    void addConstructor(FrBFunction * f)
    {
        frb_assert2(f, "FrBClass::addConstructor(FrBFunction f) -- f is null");
        
        if(f->parameterCount() == 0)
            _defaultCtor = f;
            
        _ctors.push_back(f);
    }
    void addDestructor(FrBFunction * f) { _dtor = f; }

    const ClassContainer* innerClassList() const { return &_innerClasses; }
    const FunctionContainer* functionList() const { return &_functions; }
    const ConstructorContainer* constructorList() const { return &_ctors; }
    const FrBFunction* destructor() const { return _dtor; }
    
    
    bool shared() const { return _shared; }
    bool abstract() const { return _abstract; }
    bool sealed() const { return _sealed; }
    scope_t scope() const { return _scope; }
    String name() const { return _name; }
    
    void setShared(bool v) { _shared = v; }
    void setAbstract(bool v) { _abstract = v; }
    void setSealed(bool v) { _sealed = v; }
    void setScope(scope_t v) { _scope = v; }
    void setName(String v) { _name = v; }
    
    unsigned int typeID() const { return reinterpret_cast<unsigned int>(this); }
    

};



/** FrB class from FrB code */
class FrBCodeClass : public FrBImplClass
{

public:
    FrBCodeClass();
    ~FrBCodeClass();

    FrBBaseObject * allocateInstance() const throw (FrBAllocationException)
    {
        FrBObject * o = new FrBObject(this, 0 /*nb membre */);
        /* pr chaque membre on construit */
        return o;
    }
    
    virtual const char* specString() const;
   
};


typedef std::stack<FrBCodeClass*> FrBCodeClassStack;


#endif


