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


#ifndef FRBEXECUTIONENVIRONMENT_H
#define FRBEXECUTIONENVIRONMENT_H

#include "frbbaseobject.h"
#include "frbmemory.h"

class FrBResolveEnvironment;

/** This class contains the execution environment (heap & stack...) */
class FrBExecutionEnvironment
{
private:

    /** Memory stack */
    FrBMemStack  _stack;

    /** Global memory (heap + stack) */
    FrBMemory * _mem;

    /** Shared memory (for shared data members) */
    FrBSharedMem _sharedMem;

    FrBResolveEnvironment * _resolveEnv;
    
    FrBExecutionEnvironment(const FrBExecutionEnvironment&) {}
    
public:

    /**
      * @param mem Heap memory
      * @param renv Resolve env.
      */
    FrBExecutionEnvironment(FrBMemory *mem, FrBResolveEnvironment *renv)
     : _mem(mem), _resolveEnv(renv)
    {
    }

    inline FrBResolveEnvironment * resolveEnv() { return _resolveEnv; }
    
    inline FrBMemStack& stack() { return _stack; }
    inline FrBMemory * memory() { return _mem; }
    
    /** Add the object to the FrBMemory so that it can be garbage-collected */
    inline FrBBaseObject * addGarbagedObject(FrBBaseObject * o)
    {
        _mem->addObject(*this, o);
        return o;
    }

    
    
    

};


#endif
