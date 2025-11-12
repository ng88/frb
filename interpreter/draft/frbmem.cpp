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




#include "frbmem.h"


int FrBMemManager::_unique_id = 0;

void FrBMemManager::destroy_mem_tree(MemNode * node)
{
    if(!node)
        node = _mem_base;
    
    if(!node)
        return;
           
    /* efface les enfants */
    for(ChildList::iterator it = node->childs.begin(); it != node->childs.begin(); ++it)
        destroy_mem_tree(*it);
    
    /* efface le père */
    delete node;
}


MemNode * FrBMemManager::findNode(FrBBaseObject* obj, MemNode* parent)
{
    if(!parent)
        parent = _mem_base; /* global */

    if( parent->obj == obj )
        return parent;
    
    MemNode* found = NULL;
    for(ChildList::iterator it = parent->childs.begin(); it != parent->childs.begin(); ++it)
        if( (found = findNode(obj, *it)) )
            return found;
            
    return NULL;
}

MemNode * FrBMemManager::findNode(const String& name, MemNode* parent)
{
    if(!parent)
        parent = _mem_base; /* global */

    if( parent->name == name )
        return parent;
    
    MemNode* found = NULL;
    for(ChildList::iterator it = parent->childs.begin(); it != parent->childs.begin(); ++it)
        if( (found = findNode(name, *it)) )
            return found;
            
    return NULL;
}

void FrBMemManager::show_mem_tree(MemNode * node, int depth)
{
    using namespace std;
    if(!node)
        node = _mem_base;
    
    cout << String(depth, '\t') << "|_ " << node->name << endl;
        
    int child_depth = depth + 1;
    for(ChildList::iterator it = node->childs.begin(); it != node->childs.begin(); ++it)
        show_mem_tree(*it, child_depth);
        
}

int FrBMemManager::collect(MemNode * node)
{
    if(!node)
        node = _mem_base;
    
    if( node->is_deleted && !node->childs.size() )
        return sizeof(MemNode);  /* pas d'enfant, est mort donc on delete */
    
    int total = 0;
    for(ChildList::iterator it = node->childs.begin(); it != node->childs.begin(); ++it)
        total += collect(*it);

    return total;
}


