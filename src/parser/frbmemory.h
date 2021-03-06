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



#ifndef FRBMEMORY_H
#define FRBMEMORY_H

#include "frbbaseobject.h"
#include "../common/assert.h"
#include <iostream>

//TODO au sujet de const
// c'est la zone m�moire qui doit etre const
// ou simulation de const : on copie le baseobject en le mettant const
// solution pour const :
// faire un wrapper class pr const
// mais pour ca ilfaut generaliser un peu, FrBClass aura quaisment tout
// en pure virtuel et introduire une FrBImplClass et une FrBConstClass
//
// il faut plutot trimballer les infos du types dans les FrBExpr*

class FrBExecutionEnvironment;


class FrBMemory
{
public:

    struct Block
    {
        FrBBaseObject *  value;
        int              links;
    
        Block(FrBBaseObject * v = 0)
        : value(v), links(-1) {}
    };

    static const size_t B = 1;
    static const size_t KB = 1024 * B;
    static const size_t MB = 1024 * KB;

    static const size_t BLOCK_SIZE = sizeof(Block);
    
protected:

    typedef std::vector<Block> Storage;
    
    /** Contient des instances */
    Storage _data;
    
    /** _collect_threshold is a number of block */ 
    size_t _collect_threshold;
    
    /** When the memory is full, it'll be increased of mem_size * _geo_step - mem_size */
    size_t _geo_step;
    
    /** When the memory is full, it'll be increased of _arith_step */
    size_t _arith_step;
    
    /** Mem unavailable in block */
    size_t _unavailable;
    
    /** next available block */
    size_t _next_available;
    
    void needMoreMemory();
    void findNextAvailable();
    
public:
   
    /** all parameters are in byte expect geo_step */
    inline FrBMemory(size_t init_size = MB,
		     size_t collect_threshold = MB,
		     size_t geo_step = 1,
		     size_t arith_step = MB);
    
    ~FrBMemory();


    /** add the object to the memory */
    void addObject(FrBExecutionEnvironment& e, FrBBaseObject* obj);
    
    /** juste remove the object, don't destroy it */
    inline void removeObjectFromGC(FrBBaseObject* obj);
    
    size_t collect(FrBExecutionEnvironment& e, int pass = 1);
    
    
    /** collect_threshold in byte */
    inline void setCollectThreshold(size_t s);
    
    /** size in byte */
    inline size_t size() const;

    /** size in block */
    inline size_t bsize() const;
        
    inline FrBBaseObject* getObject(int addr);
    
    inline void addLink(FrBBaseObject* obj);
    inline void delLink(FrBBaseObject* obj);
    
    std::ostream& print(int cols = 5, std::ostream& out = std::cout) const;
};




/** Represents the stack */
class FrBMemStack
{
protected:

    FrBBaseObjectList _mem;
    int               _stack_ptr;
    int               _res_step;

    void check_space(int nb);
    
public:

    FrBMemStack(int res_step = 10);
    
    /** addr is the address from top */
    inline FrBBaseObject* getTopValue(int addr) const;
    inline void setTopValue(int addr, FrBBaseObject* v);
    
    inline FrBBaseObject* top() const;
    inline FrBBaseObject* pop();
    inline void pop(int nb);
    inline void push_empty(int nb);
    inline void push(FrBBaseObject* o);
    void push(const FrBBaseObjectList& lo);
    
    inline void reserve(int nb);
    
    inline int pointer() const;
    
    std::ostream& print(std::ostream& stream = std::cout) const;
    
    inline bool overflow() const;
    

};




/** Memory used for shared object (ie shared data member) */
class FrBSharedMem
{
public:
    typedef std::map<const FrBClass*, FrBBaseObject**> Storage;
private:

    Storage _mem;

protected:

    /** Set the shared field 'index' of the class 'c' to 'o'
      * addClass(c) must be called before the first call of this function
      */
    void setSharedField(const FrBClass * c, int index, FrBBaseObject * o);

    /** Get the value of the shared field 'index' of the class 'c'
      * addClass(c) must be called before the first call of this function
      */
    FrBBaseObject * getSharedField(const FrBClass * c, int index);

public:

    FrBSharedMem();
    ~FrBSharedMem();

    
    /** Create space for shared field of the class 'c' */
    void addClass(const FrBClass * c);


    /** Set the value of the shared field 'f' to 'o'
      * addClass(f->container()) must be called before the first call of this function
      */
    inline void setSharedField(const FrBField * f, FrBBaseObject * o);


    /** Get the value of the shared field 'f'
      * addClass(f->container()) must be called before the first call of this function
      */
    inline FrBBaseObject * getSharedField(const FrBField * f);


};




























/* inlined */

/*  FrBMemory  */

inline FrBMemory::FrBMemory(size_t init_size, size_t collect_threshold, size_t geo_step, size_t arith_step)
 : _collect_threshold(collect_threshold / FrBMemory::BLOCK_SIZE + 1),
   _geo_step(geo_step),
   _arith_step(arith_step / FrBMemory::BLOCK_SIZE + 1),
   _unavailable(0),
   _next_available(0)
{
    _data.resize(init_size / FrBMemory::BLOCK_SIZE + 1);
}


inline void FrBMemory::setCollectThreshold(size_t s)
{
    _collect_threshold = s / BLOCK_SIZE + 1;
}


inline size_t FrBMemory::size() const
{
    return _data.size() * BLOCK_SIZE;
}


inline size_t FrBMemory::bsize() const
{
    return _data.size();
}
    
inline FrBBaseObject* FrBMemory::getObject(int addr)
{
    frb_assert(addr >= 0 && addr < (int)_data.size());
    return _data[addr].value;
}

inline void FrBMemory::addLink(FrBBaseObject* obj)
{
    frb_assert2(obj, "obj is a null pointer");
    frb_assert2(obj->isManaged(), "obj MUST BE a managed object");
    _data[obj->memPos()].links++;
}

inline void FrBMemory::delLink(FrBBaseObject* obj)
{
    frb_assert2(obj, "obj is a null pointer");
    frb_assert2(obj->isManaged(), "obj MUST BE a managed object");
    _data[obj->memPos()].links--;
}

inline void FrBMemory::removeObjectFromGC(FrBBaseObject* obj)
{
    frb_assert2(obj->isManaged(), "obj MUST BE a managed object");
    _data[obj->memPos()].links = -1;
}




/***** FrBMemStack *****/

inline FrBBaseObject* FrBMemStack::getTopValue(int addr) const
{
    frb_assert(addr >= 0 && addr <= _stack_ptr);
    return _mem[_stack_ptr - addr];
}

inline void FrBMemStack::setTopValue(int addr, FrBBaseObject* v)
{
    frb_assert(addr >= 0 && addr <= _stack_ptr);
    _mem[_stack_ptr - addr] = v;
}

inline FrBBaseObject* FrBMemStack::top() const
{
    frb_assert(_stack_ptr > -1 && _stack_ptr < (int)_mem.size());
    return _mem[_stack_ptr];
}

inline FrBBaseObject* FrBMemStack::pop()
{
    frb_assert(_stack_ptr >= 0);
    return _mem[_stack_ptr--];
}

inline void FrBMemStack::pop(int n)
{
    frb_assert(_stack_ptr - n >= -1);
    _stack_ptr -= n;
}

inline void FrBMemStack::reserve(int nb)
{
    check_space(nb);
}

inline void FrBMemStack::push_empty(int nb)
{
    check_space(nb);
    _stack_ptr += nb;
}

inline void FrBMemStack::push(FrBBaseObject* o)
{
    check_space(1);
    _mem[++_stack_ptr] = o;
}

inline int FrBMemStack::pointer() const
{
    return _stack_ptr;
}

inline bool FrBMemStack::overflow() const
{
    return _stack_ptr > 5000;
}

/*          FrBSharedMem             */

inline void FrBSharedMem::setSharedField(const FrBField * f, FrBBaseObject * o)
{
    frb_assert2(f->shared(), "f MUST be a shared field");
    setSharedField(f->container(), f->index(), o);
}

inline FrBBaseObject * FrBSharedMem::getSharedField(const FrBField * f)
{
    frb_assert2(f->shared(), "f MUST be a shared field");
    return getSharedField(f->container(), f->index());
}


#endif

