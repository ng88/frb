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


#include "frbmemory.h"
#include "frbclass.h"

FrBMemory::~FrBMemory()
{

    //frb_warning2(false, "memory not disposed");

    for(Storage::iterator it = _data.begin(); it != _data.end(); ++it)
        delete it->value;// il faut faire ca avec un while
        
}

void FrBMemory::needMoreMemory()
{
    //TODO to check
    _data.resize(_data.size() * _geo_step + _arith_step);
}

void FrBMemory::findNextAvailable()
{
    if(_next_available + 1 < _data.size() && _data[_next_available + 1].links == -1)
        _next_available++;
    else
    {
        for(unsigned int i = 0; i < _data.size(); ++i)
            if( _data[i].links == -1 )
            {
                _next_available = i;
                break;
            }
    }    
}

void FrBMemory::addObject(FrBExecutionEnvironment& e, FrBBaseObject* obj)
{

    if(_unavailable >= _collect_threshold)
        collect(e);
        
    if(_unavailable + 2 >= _data.size() - 1)
        needMoreMemory();
    
    _data[_next_available].value = obj;
    _data[_next_available].links = 1;
    
    obj->setMemPos(_next_available);
    
    _unavailable++;
    
    findNextAvailable();
}



size_t FrBMemory::collect(FrBExecutionEnvironment& e, int pass)
{
    size_t ret = 0;
    
    while(pass--)
    {
        for(Storage::iterator it = _data.begin(); it != _data.end(); ++it)
        {
            if( it->links == 0 )
            {
                it->value->getClass()->destroyInstance(e, it->value);
                ret++;
            }
        }
    }
    
    _unavailable -= ret;
    
    return ret * BLOCK_SIZE;
}


std::ostream& FrBMemory::print(int cols, std::ostream& out) const
{
    int nb = 1;
    
    out << "Memory size: " << size() << " bytes / " << bsize() << " blocks\n";
    out << "             (1 block = " << BLOCK_SIZE << " bytes)\n";
    
    for(Storage::const_iterator it = _data.begin(); it != _data.end(); ++it)
    {
        out << "[@" << (nb - 1) << ":" << reinterpret_cast<void*>(it->value)
                         << ',' << it->links << "]\t";
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
    _mem.resize(_res_step);

}

void FrBMemStack::check_space(int nb)
{
   if(_stack_ptr + nb > (int)_mem.size() - 1)
        _mem.resize(_stack_ptr + nb + 1 + _res_step);
}

void FrBMemStack::push(const FrBBaseObjectList& lo)
{
    check_space(lo.size());
    
    for(FrBBaseObjectList::const_reverse_iterator it = lo.rbegin(); it != lo.rend(); ++it)
        _mem[++_stack_ptr] = (*it);
}

std::ostream& FrBMemStack::print(std::ostream& stream) const
{
    stream << "Stack: " << (int)_mem.size() << std::endl;

    for(int i = (int)_mem.size() - 1; i >= 0; --i)
    {
        if(i == _stack_ptr)
            stream << " --> ";
        else
            stream << "     ";
        
        
        stream << "@" << i << " " << _mem[i] << " <var:" << (_stack_ptr - i) << ">" << std::endl;
    }
    
    return stream;
}
     

    
/*******         FrBSharedMem         *********/
//    std::map<const FrBClass*, FrBBaseObject**> _mem;

FrBSharedMem::FrBSharedMem()
{
}

FrBSharedMem::~FrBSharedMem()
{
    for(Storage::iterator it = _mem.begin(); it != _mem.end(); ++it)
	delete[] it->second;
}
   
void FrBSharedMem::addClass(const FrBClass * c)
{
    frb_assert2( _mem.find(c) == _mem.end() , "class c has already a shared storage");

    int shared_count = c->sharedFieldCount();

    FrBBaseObject ** tab = new FrBBaseObject*[shared_count];

    for(int i = 0; i < shared_count; ++i)
	tab[i] = 0;

    _mem[c] = tab;
}

void FrBSharedMem::setSharedMember(const FrBClass * c, int index, FrBBaseObject * o)
{
    frb_assert(index >= 0 && index < c->sharedFieldCount());
    frb_assert2( _mem.find(c) != _mem.end(), "class c has NOT a shared storage, call addClass() before");
    _mem[c][index] = o;
}

FrBBaseObject * FrBSharedMem::getSharedMember(const FrBClass * c, int index)
{
    frb_assert(index >= 0 && index < c->sharedFieldCount());
    frb_assert2( _mem.find(c) != _mem.end(), "class c has NOT a shared storage, call addClass() before");
    return _mem[c][index];
}
