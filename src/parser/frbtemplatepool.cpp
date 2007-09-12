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


#include "frbtemplatepool.h"

#include <sstream>
#include <string>

#include "frbclass.h"
#include "frbexpr.h"
#include "frbresolveenvironment.h"
#include "frbtemplatespecializationenvironment.h"

FrBTemplatePool::FrBInstanciedTemplateEntry::~FrBInstanciedTemplateEntry()
{
    for(FrBTypeVector::iterator it = _args.begin(); it != _args.end(); ++it)
	delete *it;
}


void FrBTemplatePool::FrBInstanciedTemplateEntry::createInstance(FrBResolveEnvironment& e)
    throw (FrBResolveException)
{
    if(_template)
	return;

    /* Resolve the template */
    _URTemplate->resolveAndCheck(e);
    _template = _URTemplate->getContext();

    frb_assert(_template);

    if(_template->templateParameterCount() != _args.size())
	throw FrBTemplateBadArgCountException(_template, _args.size());

    /* Create the new name for the instance */
    std::ostringstream sname;
    sname << _template->name();

    /* Prepare the specialize env. and complete the name */
    FrBTemplateSpecializationEnvironment env;

    for(FrBTypeVector::const_iterator it = _args.begin(); it != _args.end(); ++it)
    {
	(*it)->resolveAndCheck(e);
	env.addArgument((*it)->getContext());

	sname << (*it)->getContext()->typeID() << '$';
    }

    String name = sname.str();

    /* Does this template instance alreardy exists ? */
    const FrBClassMap * cont;
    if(_template->container())
	cont = _template->container()->innerClassList();
    else
	cont = e.classRoot();

    FrBClassMap::const_iterator f = cont->find(name);

    if(f != cont->end())
    {
        /* yes, return directly */
	_createdClass = f->second;
	return; 
    }


    /* no, we have to create it */
    FrBClass * c = _template->specializeTemplate(env);

    _createdClass = c;

    if(_template->container())
	c->containerPtr()->addInnerClass(c, name);
    else
	e.classRoot()->insert(std::make_pair(name, c));
}

FrBTemplatePool::~FrBTemplatePool()
{
    for(FrBITEList::iterator it = _list.begin(); it != _list.end(); ++it)
	delete *it;
}


void FrBTemplatePool::createInstances(FrBResolveEnvironment& e)
    throw (FrBResolveException)
{
    for(FrBITEList::iterator it = _list.begin(); it != _list.end(); ++it)
	(*it)->createInstance(e);
}

