/***************************************************************************
 *   This file is part of the FrB project                                  *
 *                                                                         *
 *   FrB project                                                           *
 *                                                                         *
 *   Copyright (C) 2006 - 2010 by GUILLAUME Nicolas                        *
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

#include <iostream>
FrBTemplatePool::FrBInstanciedTemplateEntry::~FrBInstanciedTemplateEntry()
{
    for(FrBTypeVector::iterator it = _args.begin(); it != _args.end(); ++it)
	delete_expr(*it);
}

void FrBTemplatePool::FrBInstanciedTemplateEntry::createInstance(FrBResolveEnvironment& e)
    throw (FrBResolveException)
{
    if(_template)
    {
	frb_assert(_createdClass);
	return;
    }

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
	std::cout << "e="<< *(*it) << std::endl;
	env.addArgument((*it)->getContext());

	sname << (*it)->getContext()->typeID() << '$';
    }

    String name = sname.str();

    /* Does this template instance alreardy exists ? */
    _createdClass  = e.findTemplate(name);

    if(_createdClass)
        /* yes, return directly */
	return; 

    /* no, we have to create it */
    FrBClass * c = _template->specializeTemplate(env);

    _createdClass = c;

    e.addTemplate(c, name);

    /* now we resolve it */
    c->resolvePrototype(e);
    //c->resolveAndCheck(e);

}

FrBTemplatePool::~FrBTemplatePool()
{
    frb_assert(_stack.empty());

    for(FrBITEList::iterator it = _list.begin(); it != _list.end(); ++it)
	delete *it;
}

#include <iostream>
void FrBTemplatePool::resolveAndCheckCreatedClasses(FrBResolveEnvironment& e)
    throw (FrBResolveException)
{
    frb_warning2(false, "enter FrBTemplatePool::resolveAndCheckCreatedClasses()");
    std::cout << _list.size() << "\n";
    frb_assert2(false, "deprecated");
    for(FrBITEList::iterator it = _list.begin(); it != _list.end(); ++it)
    {
	if(!(*it)->classCreated())
	{
	    //const_cast<FrBClass*>((*it)->getCreatedClass())->resolvePrototype(e);
	    //const_cast<FrBClass*>((*it)->getCreatedClass())->resolveAndCheck(e);
	    (*it)->createInstance(e);
	}

    }
}

