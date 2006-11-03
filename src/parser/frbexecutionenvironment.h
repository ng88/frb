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

class FrBResolveEnvironment;

/** This class contains the execution environment (heap & stack...) */
class FrBExecutionEnvironment
{
private:

    FrBMemStack  _stack;
    FrBMemory * _mem;
    FrBResolveEnvironment * _resolveEnv;
    FrBBaseObject * _outsideMe;
    
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

    /** used for expression that're outside function and need a Me value */
    inline FrBBaseObject * outsideMe() { return _outsideMe; }
    inline void setOutsideMe(FrBBaseObject * m) { _outsideMe = m; }
    
    /** Add the object to the FrBMemory so that it can be garbage-collected */
    inline void addGarbagedObject(FrBBaseObject * o)
    {
        _mem->addObject(*this, o);
    }

    
    
    

};


#endif
