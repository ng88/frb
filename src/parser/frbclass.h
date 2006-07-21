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

class FrBClass /* a frb class in memory */
{
public:

    typedef std::multimap<const String, FrBFunction*>   FunctionContainer;
    typedef std::multimap<int, FrBFunction*>            OperatorContainer;
    typedef FrBFunctionList                             ConstructorContainer;
    typedef FrBClassMap                                 ClassContainer;
    typedef FunctionContainer::const_iterator           FnContIt;
    typedef std::pair<FnContIt, FnContIt>               FnPairIt;
    
protected:



    bool     _shared; /* shared class, ie module */
    bool     _abstract; /* abstract class, ie interface */
    bool     _sealed; /* sealed class */

    scope_t  _scope;
    String   _name;

    FunctionContainer _functions;
    ClassContainer _innerClasses;
    
    OperatorContainer _operators;
    
    ConstructorContainer _ctors;
    FrBFunction * _defaultCtor;
    FrBFunction * _dtor;

public:

    FrBClass()
     : _shared(false), _abstract(false), 
       _sealed(false), _scope(SC_PRIVATE),_defaultCtor(0), _dtor(0) {}
    
    virtual ~FrBClass();
    
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
             
    template<class ArgContainer>
    inline FrBFunction * findConstructor(const ArgContainer& args) const
          throw (FrBFunctionNotFoundException)
    {
        return FrBFunction::findOverload(_ctors.begin(), _ctors.end(), args);
    }
    
    inline FnPairIt findFunctions(const String& name) const
    {
        return _functions.equal_range(name);
    }
    
    template<class ArgContainer>
    inline FrBFunction * findFunction(const String& name, const ArgContainer& args) const
         throw (FrBFunctionNotFoundException)
    {
        FnPairIt seq = _functions.equal_range(name);
        
        return FrBFunction::findOverload(
                const_map_snd_iterator<FunctionContainer>(seq.first),
                const_map_snd_iterator<FunctionContainer>(seq.second),
                args);
    }
    
    template<class ArgContainer>
    inline FrBFunction * findOperator(int op, const ArgContainer& args) const
         throw (FrBFunctionNotFoundException)
    {
        typedef OperatorContainer::const_iterator It;
        std::pair<It, It> seq = _operators.equal_range(op);
        
        return FrBFunction::findOverload(
                const_map_snd_iterator<OperatorContainer>(seq.first),
                const_map_snd_iterator<OperatorContainer>(seq.second),
                args);
    }
    
    inline FrBFunction * findDestructor() const
        throw (FrBFunctionNotFoundException) 
    {
        if(_dtor == 0)
            throw FrBDtorNotFoundException();
            
        return _dtor;
    }
    
    /** Execute the default constructor (with no param). If no ctor exists and if there are no other ctor, a default ctor is created and executed */
    void executeDefaultConstructor(FrBBaseObject * me) const throw (FrBExecutionException);
    
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
        
    /*virtual FrBBaseObject * callOperator(String name, FrBBaseObjectList args) = 0;
    virtual FrBBaseObject * getProperty(String name, FrBBaseObjectList args) = 0;
    virtual FrBBaseObject * setProperty(String name, FrBBaseObjectList args) = 0;// faire une binaryOperator pr optimiser
    virtual FrBBaseObject * getMember(String name) = 0;*/
    
    //TODO /* throw si existe deja */
    inline void addInnerClass(FrBClass * c) { _innerClasses[c->name()] = c; }
    inline void addFunction(FrBFunction * f) { _functions.insert(std::make_pair(f->name(), f)); }
    //TODO vérifier que au moins l'un des params concerne la classe (ie pour int il faut 
    // op(int, int) op(int, double) ou op(double, int) mais pas op(double, single)
    inline void addOperator(int op, FrBFunction * f) { _operators.insert(std::make_pair(op, f)); }
    //TODO /* verifier la validité des params et retour */
    inline void addConstructor(FrBFunction * f)
    {
        frb_assert2(f, "FrBClass::addConstructor(FrBFunction f) -- f is null");
        
        if(f->parameterCount() == 0)
            _defaultCtor = f;
            
        _ctors.push_back(f);
    }
    inline void addDestructor(FrBFunction * f) { _dtor = f; }

    inline const ClassContainer* innerClassList() const { return &_innerClasses; }
    inline const OperatorContainer* operatorList() const { return &_operators; }
    inline const FunctionContainer* functionList() const { return &_functions; }
    inline const ConstructorContainer* constructorList() const { return &_ctors; }
    inline const FrBFunction* destructor() const { return _dtor; }
    
    
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
    
    /** Check if from and to are compatibles and convert from to to */
    static FrBBaseObject* convert(FrBBaseObject * from, const FrBClass * to)throw (FrBIncompatibleClassException);
    
    /** Convert from to to without check */
    static FrBBaseObject* forceConvert(FrBBaseObject * from, const FrBClass * to);
    
    /** Check if from and to are compatibles */
    static bool areCompatibles(const FrBClass * from, const FrBClass * to);
    
};



/** FrB class from FrB code */
class FrBCodeClass : public FrBClass
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

