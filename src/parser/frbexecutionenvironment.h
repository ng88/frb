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

#include "../common/assert.h"
#include "frbbaseobject.h"
#include "frbmemory.h"
#include <map>

class FrBFunction;

class FrBResolveEnvironment;

/** This class contains the execution environment (heap & stack...) */
class FrBExecutionEnvironment
{
public:

    /** Represent a event for an instance of a class */
    class FrBEventInstance
    {
    public:
	const FrBFunction * event;
	FrBBaseObject * instance;


	bool operator<(const FrBEventInstance&) const;
	bool operator==(const FrBEventInstance&) const;

	FrBEventInstance(FrBBaseObject * inst, const FrBFunction * e);
	FrBEventInstance(const FrBEventInstance&);

    };

    /** (signal, instance) -> (slot, instance) */
    typedef std::multimap<FrBEventInstance, FrBEventInstance> FrBEventPool;
    typedef std::pair<FrBEventPool::const_iterator, FrBEventPool::const_iterator>  FrBEventPairIterator;

private:

    /** Memory stack */
    FrBMemStack  _stack;

    /** Global memory (heap + stack) */
    FrBMemory * _mem;

    /** Shared memory (for shared data members) */
    FrBSharedMem _sharedMem;


    /** Event pool */
    FrBEventPool _eventPool;

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
    inline FrBSharedMem& sharedMem() { return _sharedMem; }
    inline FrBMemory * memory() { return _mem; }
    
    /** Add the object to the FrBMemory so that it can be garbage-collected */
    inline FrBBaseObject * addGarbagedObject(FrBBaseObject * o)
    {
        _mem->addObject(*this, o);
        return o;
    }


    /** Register an event in the event pool
      * @param instance instance of the class concerned by the registration. If this parameter is FrBNull,
      * the registration concern all the instance. Note: instance->getClass() must be compatible with event->getContainer()
      * @param event event
      * @param handler function that will be called when event is raised
      */
    void registerEvent(FrBBaseObject * callerInstance, const FrBEvent * signal, FrBBaseObject * calledInstance, const FrBFunction * slot);
 
    void unregisterEvent(FrBBaseObject * callerInstance, const FrBEvent * signal, FrBBaseObject * calledInstance, const FrBFunction * slot);

    void raiseEvent(FrBBaseObject * callerInstance, const FrBEvent * signal, const FrBBaseObjectList& args)
	throw (FrBExecutionException);
    
    
    

};


#endif
