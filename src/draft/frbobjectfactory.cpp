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


