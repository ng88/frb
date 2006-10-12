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

typedef std::vector<FrBBaseObject*> FrBBaseObjectStack;

class FrBMemory;

    //TODO fournit ici la liste des classes, valables aussi pr les autres sous classes


/** This class contains the execution environment (heap & stack...) */
class FrBExecutionEnvironment
{
private:

    FrBBaseObjectStack  _stack;
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
    
    
    inline FrBBaseObject* getStackValue(int address) { return _stack[address]; }
    inline void setStackValue(int address, FrBBaseObject* v) { _stack[address] = v;; }
    
    inline void popStack() { _stack.pop_back(); }
    inline void pushStack(FrBBaseObject* o) { _stack.push_back(o); }
    
    inline void pushStack(FrBBaseObjectList* lo)
    {
        for(FrBBaseObjectList::iterator it = lo->begin(); it != lo->end(); ++it)
            _stack.push_back(*it);
    }
    
    
    
    inline FrBMemory * memory() { return _mem; }
    
    

};


#endif
