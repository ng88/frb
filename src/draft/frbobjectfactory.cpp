
#include "frbobjectfactory.h"

void __objFactory__destroy()
{
    delete ptrFrbTypes;
    ptrFrbTypes = NULL;
    
    std::cout << "__objFactory__destroy()\n";
}

ObjectFactory::ObjectFactory(const String& name, FrBObjectCreator fn)
{
    if( ptrFrbTypes == NULL )
    {
        ptrFrbTypes = new FrBCreatorList();
        atexit( __objFactory__destroy );
    }
    
    if( (*ptrFrbTypes)[name] )
    {
        std::cerr << "Type " << name << " already declared !\n";
        abort();
    }
    
    (*ptrFrbTypes)[name] = fn;
    _id = ptrFrbTypes->size();
    
    std::cout << "ObjectFactory() - " << name << " - " << _id << "\n";
}

FrBBaseObject * ObjectFactory::newObject(const String& type)
{
    std::cout << "newObject() - " << type << "\n";

    FrBObjectCreator fn = (*ptrFrbTypes)[type];
    
    if(fn)
        return fn();
    else
        return NULL;
}

/*FrBBaseObject* ObjectFactory::__callFunction(FrBBaseObject * obj, void * fn, FrBArgList args)
{
    for(FrBArgList::iterator it = args.begin(); it != args.end(); ++it)
        asm( "push %[in]" :  : [in] "d" ( (*it) ) );

    if( obj != NULL )
        asm( "push %[in]" :  : [in] "d" (obj) );
        
    asm( "call *%[in]" :  : [in] "d" (fn) );
    
    return NULL;
}*/


