/***************************************************************************
 *   This file is part of the FrB project                                  *
 *                                                                         *
 *   FrB project                                                           *
 *                                                                         *
 *   Copyright (C) 2006 by GUILLAUME Nicolas                               *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://frb.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   See the COPYING file.                                                 *
 ***************************************************************************/


#include "frbmemory.h"
#include "../common/assert.h"


FrBMemory * FrBMemory::_mem = new FrBMemory();

FrBMemory::~FrBMemory()
{

    for(Storage::iterator it = _data.begin(); it != _data.end(); ++it)
        ;//delete *it;
        
    frb_warning2(false, "memory not disposed");
}


FrBBaseObject* FrBMemory::addObject(const String& name, FrBBaseObject* obj)
{
    /* en cas de succès */
    
    if(_data.size() >= _collect_threshold)
        collect();
    
    _data[name] = FrBVar(obj);
    return obj;
}

void FrBMemory::deleteObject(FrBBaseObject* obj)
{
    delete obj;
}



size_t FrBMemory::collect(int pass)
{
    size_t ret = 0;
    
    while(pass--)
    {
        for(Storage::iterator it = _data.begin(); it != _data.end(); ++it)
        {
            if( it->second.links == 0 )
            {
                //on delete
                ret++;
            }
        }
    }
    
    return ret * BLOCK_SIZE;
}


std::ostream& FrBMemory::print(int cols, std::ostream& out) const
{
    int nb = 1;
    
    out << "Memory size: " << size() << " bytes / " << bsize() << " blocks\n";
    out << "             (1 block = " << BLOCK_SIZE << " bytes)\n";
    
    for(Storage::const_iterator it = _data.begin(); it != _data.end(); ++it)
    {
        out << '[' <<  it->first << '@' << reinterpret_cast<void*>(it->second.value)
                         << ':' << it->second.links << "]\t";
        nb++;
        
        if(nb % cols == 0) out << '\n';
    }
    
    if(nb % cols != 0) out << '\n';
    
    return out;
}

/**** stack ***********/


FrBMemStack::FrBMemStack(int res)
{
    _res_step = res;
    _stack_ptr = -1;
    _mem.reserve(_res_step);

#ifdef _FRB_DEBUG_
    for(unsigned int i = 0; i < _mem.size(); ++i)
        _mem[i] = 0;
#endif

}

void FrBMemStack::check_space(int nb)
{
    if(_stack_ptr + nb > (int)_mem.size() - 1)
        _mem.reserve(_stack_ptr + nb + 1 + _res_step);
        
#ifdef _FRB_DEBUG_
    for(unsigned int i = _stack_ptr + 1; i < _mem.size(); ++i)
        _mem[i] = 0;
#endif
        
}

void FrBMemStack::push(const FrBBaseObjectList& lo)
{
    check_space(lo.size());
    
    for(FrBBaseObjectList::const_iterator it = lo.begin(); it != lo.end(); ++it)
        _mem[++_stack_ptr] = (*it);
}

std::ostream& FrBMemStack::print(std::ostream& stream) const
{
    stream << "Stack:" << std::endl;

    for(int i = _mem.size() - 1; i > -1; --i)
    {
        if(i == _stack_ptr)
            stream << " --> ";
        else
            stream << "     ";
        
        
        stream << "@" << i << " " << _mem[_stack_ptr] << " <var:" << (_stack_ptr - i) << ">" << std::endl;
    }
    
    return stream;
}
     
    
