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


#ifndef FRBTEMPLATEPOOL_H
#define FRBTEMPLATEPOOL_H


#include <stack>
#include <vector>
#include <list>


#include "../common/assert.h"
#include "frbexceptions.h"

class FrBClass;
class FrBTypeExpr;
class FrBResolveEnvironment;

/**
 * An instance of this class contains the list of the template that will be instancied
 * juste before resolve&check.
 *
 *
 * Templates work as follow:
 *   1) At parse time: first a FrBTemplatePool is created, then, when we
 *   see an expr like A<T>, we add an FrBInstanciedTemplateEntry in this pool.
 *   2) At resolve time: juste before resolving anything we call
 *   FrBTemplatePool::createInstances which creates the needed template instance
 *   by calling template->specializeTemplate();
 */
class FrBTemplatePool
{
public:

    typedef std::vector<FrBTypeExpr*> FrBTypeVector;
    class FrBInstanciedTemplateEntry
    {
    private:
	FrBTypeVector  _args;
	FrBTypeExpr  * _URTemplate;

	const FrBClass* _template;
	const FrBClass* _createdClass;
    public:
	inline FrBInstanciedTemplateEntry(FrBTypeExpr *);
	~FrBInstanciedTemplateEntry();

	inline void addArg(FrBTypeExpr* a);

	/** Create the template instance (and add it to the resolve env.) */
	void createInstance(FrBResolveEnvironment&) throw (FrBResolveException);
	
	/** must be called after createInstance() */
	inline const FrBClass* getCreatedClass() const;

	inline FrBTypeExpr* getArg(unsigned int i) const;
	inline unsigned int argCount() const;
	inline const FrBClass* getTemplate() const;
	inline FrBTypeExpr* getURTemplate() const;

	inline bool classCreated() const;
    };

    typedef std::stack<FrBInstanciedTemplateEntry*> FrBITEStack;
    typedef std::list<FrBInstanciedTemplateEntry*>  FrBITEList;

private:

    FrBITEStack _stack;
    FrBITEList _list;
    
public:

    ~FrBTemplatePool();

//parser::parse() appelle une methode d'ici en passant le resolveEnv et ca fait tout le boulot
// remplacer les _template par cette pool
// createInstances doit verifier le nombre de param et eventuellement jeter une exception

    /** Add a new instanciation entry for class c.
     *  Template argument must be provided by addArgument().
     *  Return the created entry.
     */
    inline FrBInstanciedTemplateEntry * pushEntry(FrBTypeExpr*);

    /** Add an argument to the current entry */
    inline void addArgument(FrBTypeExpr* a);

    /** Pop the current entry. */
    inline void popEntry();

    /** Create all the template instances added to this pool.
     * The created instance are added to e
     */
    void  resolveAndCheckCreatedClasses(FrBResolveEnvironment& e) throw (FrBResolveException);


};





















/*     inlined  */


inline FrBTemplatePool::FrBInstanciedTemplateEntry::FrBInstanciedTemplateEntry(FrBTypeExpr * c)
    : _URTemplate(c), _template(0)
{
    frb_assert(c);
    FRB_ASSERT_CODE( _createdClass = 0 );
}
		  
inline void FrBTemplatePool::FrBInstanciedTemplateEntry::addArg(FrBTypeExpr* a)
{
    frb_assert(a);
    
#ifdef DEBUG_ASSERT
    for(FrBTypeVector::iterator it = _args.begin(); it != _args.end(); ++it)
	frb_assert2((*it) != a, "a already in _args!");
#endif

    _args.push_back(a);
}

inline const FrBClass* FrBTemplatePool::FrBInstanciedTemplateEntry::getCreatedClass() const
{
    frb_assert2(_createdClass, "call createInstance() before!");
    return _createdClass;
}

inline bool FrBTemplatePool::FrBInstanciedTemplateEntry::classCreated() const
{
    return _createdClass != 0;
}

inline const FrBClass* FrBTemplatePool::FrBInstanciedTemplateEntry::getTemplate() const
{
    frb_assert2(_template, "template not resolved");
    return _template;
}

inline FrBTypeExpr* FrBTemplatePool::FrBInstanciedTemplateEntry::getURTemplate() const
{
    return _URTemplate;
}

inline FrBTypeExpr* FrBTemplatePool::FrBInstanciedTemplateEntry::getArg(unsigned int i) const
{
    frb_assert(i < argCount());
    return _args[i];
}

inline unsigned int FrBTemplatePool::FrBInstanciedTemplateEntry::argCount() const
{
    return _args.size();
}

inline FrBTemplatePool::FrBInstanciedTemplateEntry * FrBTemplatePool::pushEntry(FrBTypeExpr*c)
{
    frb_assert(c);

    FrBInstanciedTemplateEntry * e = new FrBInstanciedTemplateEntry(c);
    _stack.push(e);
    _list.push_back(e);
    return e;
}


inline void FrBTemplatePool::addArgument(FrBTypeExpr* a)
{
    frb_assert(a);
    frb_assert(!_stack.empty());
    _stack.top()->addArg(a);
}

inline void FrBTemplatePool::popEntry()
{
    frb_assert(!_stack.empty());
    _stack.pop();
}



#endif

