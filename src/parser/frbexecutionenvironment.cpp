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


#include "frbexecutionenvironment.h"
#include "frbresolveenvironment.h"
#include "frbbuiltinclasses.h"

#include "frbfunction.h"

bool FrBExecutionEnvironment::FrBEventInstance::operator<(const FrBEventInstance& o) const
{
    return event < o.event;
}

bool FrBExecutionEnvironment::FrBEventInstance::operator==(const FrBEventInstance& o) const
{
    return event == o.event && instance == o.instance;
}

FrBExecutionEnvironment::FrBEventInstance::FrBEventInstance(const FrBEventInstance& o)
   : event(o.event), instance(o.instance)
{
}


FrBExecutionEnvironment::FrBEventInstance::FrBEventInstance(FrBBaseObject * inst, const FrBFunction * e)
   : event(e), instance(inst)
{
    frb_assert(instance && event);
    frb_assert2( FrBNull::isNull(instance) || instance->getClass()->isCompatibleWith(event->container()), "invalid instance");
}

    
void FrBExecutionEnvironment::registerEvent(FrBBaseObject * callerInstance, const FrBFunction * signal, FrBBaseObject * calledInstance, const FrBFunction * slot)
{

    FrBEventInstance caller(callerInstance, signal);
    FrBEventInstance called(calledInstance, slot);

    _eventPool.insert(std::make_pair(caller, called)); 

}

void FrBExecutionEnvironment::unregisterEvent(FrBBaseObject * callerInstance, const FrBFunction * signal, FrBBaseObject * calledInstance, const FrBFunction * slot)
{

    FrBEventInstance caller(callerInstance, signal);
    FrBEventInstance called(calledInstance, slot);

    FrBEventPairIterator seq = _eventPool.equal_range(caller);

    for(FrBEventIterator it = seq.first ; it != seq.second; ++it)
	if( it->second == called )
	{
	    _eventPool.erase(it);
	    break;
	}


}

void FrBExecutionEnvironment::raiseEvent(FrBBaseObject * callerInstance, const FrBFunction * signal, const FrBBaseObjectList& args)
    throw (FrBExecutionException)
{

    FrBEventInstance caller(callerInstance, signal);

    FrBEventPairIterator seq = _eventPool.equal_range(caller);

    for(FrBEventIterator it = seq.first ; it != seq.second; ++it)
	if(it->second.event->event())
	    raiseEvent(it->second.instance, it->second.event, args);
	else
	    it->second.event->execute(*this, it->second.instance, args);

}

