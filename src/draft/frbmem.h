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



#ifndef FRBMEM_H
#define FRBMEM_H

#include "frbobjectfactory.h"

#include <vector>
#include <iostream>

#define FRB_MEM_GLOBAL "$Global$"

#define UNIQUE_NAME (StringEx::int2string(_unique_id++) + "__tmp_boxing_var_$+$")

struct MemNode;
class MemManager;

typedef std::vector<MemNode*> ChildList;

struct MemNode
{
private:
    MemNode(MemNode&) {}
    
public:
    ChildList childs; /* enfants */
    String name; /* unique par fils */
    FrBBaseObject * obj; /* peut etre NULL si on def. un block... */
    bool is_deleted;
    
    inline MemNode(MemNode *p = NULL, String n = String(), FrBBaseObject *o = NULL)
     : childs(ChildList()), name(n), obj(o), is_deleted(false)
     {
         if(p) p->childs.push_back(this);
     }
    
    inline void kill()
    {
        is_deleted = true;
        ObjectFactory::deleteObject(obj);
        obj = NULL;
        
        if( !childs.size() )
            delete this; /* detruit definitivement qui se y a pas d'enfants */
                         /* sinon le garbage collector s'en chargera */
                         
    }
    
    inline ~MemNode() { ObjectFactory::deleteObject(obj); }  /* ne devrait jamais arriver */
};


class FrBMemManager
{
private:

    static int _unique_id;    

    MemNode * _mem_base; /* pointe vers le parent de base */

    FrBMemManager(FrBMemManager&) {}
    
    void destroy_mem_tree(MemNode * node = NULL);
public:

    inline FrBMemManager() { _mem_base = new MemNode(NULL, FRB_MEM_GLOBAL, NULL); }
    inline ~FrBMemManager() { destroy_mem_tree(_mem_base); }

    inline MemNode * createNewObject(const String& type, const String& name, MemNode* parent = NULL)
        { return new MemNode( (parent ? parent : _mem_base), name, ObjectFactory::newObject(type) ); }
    inline MemNode * createNewObject(const String& type, const String& name, const String& parent = String())
        { return new MemNode( (parent.length() ? findNode(parent) : _mem_base), name, ObjectFactory::newObject(type) ); }
    inline MemNode * createNewObject(const String& type, const String& name, FrBBaseObject* parent = NULL)
        { return new MemNode( (parent ? findNode(parent) : _mem_base), name, ObjectFactory::newObject(type) ); }
    inline void destroyObject(MemNode* obj) { if(obj) obj->kill(); }
    inline void destroyObject(FrBBaseObject* obj) { destroyObject( findNode(obj) ); }
    inline void destroyObject(const String& name) { destroyObject( findNode(name) ); }
    
    inline MemNode * createNamespace(const String& name, MemNode* parent = NULL)
        { return new MemNode( (parent ? parent : _mem_base) , name ); }
    inline MemNode * createNamespace(const String& name, const String& parent = String())
        { return new MemNode( (parent.length() ? findNode(parent) : _mem_base) , name ); }
    inline void destroyNamespace(MemNode* ns) { destroyObject(ns); }          /* alias */
    inline void destroyNamespace(const String& name) { destroyObject(name); } /* alias */
    
    MemNode * findNode(FrBBaseObject* obj, MemNode* parent = NULL);
    MemNode * findNode(const String& name, MemNode* parent = NULL);
    
    void show_mem_tree(MemNode * node = NULL, int depth = 0);
    
    MemNode * boxInt(int v) { return createNewObject("Int", UNIQUE_NAME, _mem_base); }
    
    int collect(MemNode * node = NULL); /* garbage collector */
    
    
};


#endif
