#include "frbclass.h"
#include "frbmemory.h"

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
    const ClassContainer * inners = innerClassList();
    
    for(FunctionContainer::const_iterator itf = fns->begin(); itf != fns->end(); ++itf)
            itf->second->put(sout, level);
        
    sout << ident << "\t+ Inner classes:" << endl;
    
    for(ClassContainer::const_iterator it = inners->begin(); it != inners->end(); ++it)
        it->second->put(sout, level + 1);

    return sout;
}

FrBBaseObject * FrBClass::createInstance() const throw (FrBExecutionException)
{
    FrBBaseObject * o = allocateInstance();
    
    executeDefaultConstructor(o);
   
        
    return o;
}

FrBBaseObject * FrBClass::createInstance(const FrBBaseObjectList& args) const throw (FrBExecutionException)
{
    FrBBaseObject * o = allocateInstance();
    executeConstructor(o, args);
    return o;
}

void FrBClass::destroyInstance(FrBBaseObject * o) const throw (FrBExecutionException)
{
    executeDestructor(o);
    FrBMemory::getMemory()->deleteObject(o);
}

FrBClass::~FrBClass()
{

}

/*           FrBImplClass                  */

void FrBImplClass::executeDefaultConstructor(FrBBaseObject * me) const throw (FrBExecutionException)
{
    if(_defaultCtor)
    {
        _defaultCtor->execute(me);
    }
    else
    {
        if(_ctors.size() > 0)
            throw FrBDefaultCtorNotFoundException();
        else
            ;//TODO  default ctor creation & execution
    }
        
}




FrBImplClass::~FrBImplClass()
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
}


/*        FrBCodeClass            */

FrBCodeClass::FrBCodeClass() {};

FrBCodeClass::~FrBCodeClass()
{

};

const char* FrBCodeClass::specString() const
{
    return "user code class";
}


    
    
