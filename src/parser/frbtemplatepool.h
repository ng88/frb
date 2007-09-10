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
	const FrBClass* _template;
	const FrBClass* _createdClass;
    public:
	inline FrBInstanciedTemplateEntry(const FrBClass* c);
	~FrBInstanciedTemplateEntry();

	inline void addArg(FrBTypeExpr* a);

	/** Create the template instance (and add it to the resolve env.) */
	void createInstance(FrBResolveEnvironment&) throw (FrBResolveException);
	
	/** must be called after createInstance() */
	inline const FrBClass* getCreatedClass() const;
    };

    typedef std::stack<FrBInstanciedTemplateEntry*> FrBITEStack;
    typedef std::vector<FrBInstanciedTemplateEntry*>  FrBITEVector;

private:

    FrBITEStack _stack;
    FrBITEVector _list;
    
public:

    ~FrBTemplatePool();

//parser::parse() appelle une methode d'ici en passant le resolveEnv et ca fait tout le boulot
// remplacer les _template par cette pool
// createInstances doit verifier le nombre de param et eventuellement jeter une exception

    /** Add a new instanciation entry for class c.
     *  Template argument must be provided by addArgument().
     *  Return an id usable with getCreatedClass().
     */
    inline unsigned int pushEntry(const FrBClass* c);

    /** Add an argument to the current entry */
    inline void addArgument(FrBTypeExpr* a);

    /** Pop the current entry. */
    inline void popEntry();

    /** Create all the template instances added to this pool.
     * The created instance are added to e
     */
    void createInstances(FrBResolveEnvironment& e) throw (FrBResolveException);

    /**
     * Return the created class #id. must be called after createInstances()
     */
    inline const FrBClass * getCreatedClass(unsigned int id) const;
};





















/*     inlined  */


inline FrBTemplatePool::FrBInstanciedTemplateEntry::FrBInstanciedTemplateEntry(const FrBClass* c)
    : _template(c)
{
    frb_assert(c);
    FRB_ASSERT_CODE( _createdClass = 0 );
    _args.reserve(c->templateParameterCount());
}
		  
inline void FrBTemplatePool::FrBInstanciedTemplateEntry::addArg(FrBTypeExpr* a)
{
    frb_assert(a);
    _args.push_back(a);
}

inline const FrBClass* FrBTemplatePool::FrBInstanciedTemplateEntry::getCreatedClass() const
{
    frb_assert2(_createdClass, "call createInstance() before!");
    return _createdClass;
}

inline unsigned int FrBTemplatePool::pushEntry(const FrBClass* c)
{
    frb_assert(c);

    FrBInstanciedTemplateEntry * e = new FrBInstanciedTemplateEntry(c);
    _stack.push(e);
    _list.push_back(e);
    return _list.size() - 1;
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

inline const FrBClass *  FrBTemplatePool::getCreatedClass(unsigned int  id) const
{
    return _list[id]->getCreatedClass();
}


#endif

