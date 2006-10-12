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


#ifndef FRBEXECUTIONENVIRONMENT_H
#define FRBEXECUTIONENVIRONMENT_H

#include "frbbaseobject.h"
#include "frbmemory.h"


    //TODO fournit ici la liste des classes, valables aussi pr les autres sous classes


/** This class contains the execution environment (heap & stack...) */
class FrBExecutionEnvironment
{
private:

    FrBMemStack  _stack;
    FrBMemory * _mem;
    
    FrBExecutionEnvironment(const FrBExecutionEnvironment&) {}
    
public:

    /**
      * @param mem Heap memory
      */
    FrBExecutionEnvironment(FrBMemory *mem)
     : _mem(mem)
    {
    }
    
    
    inline FrBMemStack& stack() { return _stack; }
    inline FrBMemory * memory() { return _mem; }
    
    /** Add the object to the FrBMemory so that it can be garbage-collected */
    inline void addGarbagedObject(FrBBaseObject * o)
    {
        //TODO
    }
    
    /** Del the object to the FrBMemory (will be collected as soon as possible)*/
    inline void delGarbagedObject(FrBBaseObject * o)
    {
        //TODO
    }
    
    

};


#endif
