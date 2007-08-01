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

#include "frbfunction.h"



voidr FrBExecutionEnvironment::registerEvent(FrBBaseObject * instance, FrBEvent * event, FrBFunction * handler)
{
    frb_assert(handler);

    FrBEventInstance inst(instance, event);

    _eventPool.insert(std::make_pair(inst, handler)); 

}

void FrBExecutionEnvironment::unregisterEvent(FrBBaseObject * instance, FrBEvent * event)
{

    FrBEventInstance inst(instance, event);

    _eventPool.erase(inst);

}

void FrBExecutionEnvironment::raiseEvent(FrBBaseObject * instance, FrBEvent * event, FrBBaseObject * caller, const FrBBaseObjectList& args) throw (FrBExecutionException)
{

    FrBEventInstance inst(instance, event);

    FrBEventPairIterator seq = _eventPool.equal_range(inst);

    for(; seq.first != seq.second; seq.first++)
	seq.first->second->execute(*this, caller, args);

}

