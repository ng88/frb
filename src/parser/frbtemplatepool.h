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


#ifndef FRBTEMPLATEPOOL_H
#define FRBTEMPLATEPOOL_H

class FrBClass;
class FrBTypeExpr;

#include <stack>
#include <vector>
#include <list>

/**
 * An instance of this class contains the list of the template that will be instancied
 * juste before resolve&check.
 */
class FrBTemplatePool
{
public:

    typedef std::vector<FrBTypeExpr*> FrBTypeVector;
    class FrBInstanciedTemplateEntry
    {
    private:
	FrBTypeVector  _args;
	const FrBClass* _template;
    public:
	inline FrBInstanciedTemplateEntry(const FrBClass* c);

	inline void addArg(FrBTypeExpr* a);

	FrBClass * getInstance();
	
    };

    typedef std::stack<FrBInstanciedTemplateEntry*> FrBITEStack;
    typedef std::list<FrBInstanciedTemplateEntry*>  FrBITEList;
private:

    FrBITEStack _stack;
    FrBITEList _list;
    
public:

//parser::parse() appelle une methode d'ici en passant le resolveEnv et ca fait tout le boulot
//
};


#endif


















/*     inlined  */
inline FrBTemplatePool::FrBInstanciedTemplateEntry::FrBInstanciedTemplateEntry(const FrBClass* c)
    : _template(c)
{
    frb_assert(c);
    _args.reserve(c->templateParameterCount());
}
		  
inline void FrBTemplatePool::FrBInstanciedTemplateEntry::addArg(FrBTypeExpr* a)
{
    frb_assert(a);
    _args.push_back(a);
}
