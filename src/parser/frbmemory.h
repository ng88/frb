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



#ifndef FRBMEMORY_H
#define FRBMEMORY_H

#include "frbclass.h"


//TODO au sujet de const
// c'est la zone mémoire qui doit etre const
// ou simulation de const : on copie le baseobject en le mettant const
// solution pour const :
// faire un wrapper class pr const
// mais pour ca ilfaut generaliser un peu, FrBClass aura quaisment tout
// en pure virtuel et introduire une FrBImplClass et une FrBConstClass


struct FrBVar
{
    const FrBClass * type; //TODO virer ca car inutile (double verif)
    FrBBaseObject *  value;
    int              links;
    
    FrBVar()
    : type(0), value(0), links(0) {}
    
    FrBVar(const FrBClass * c, FrBBaseObject * v)
     : type(c), value(v), links(0) {}
};

class FrBMemory
{
public:
    static const size_t B = 1;
    static const size_t KB = 1024 * B;
    static const size_t MB = 1024 * KB;

    static const size_t BLOCK_SIZE = sizeof (FrBVar);
    
private:
    static FrBMemory * _mem;
    

    
    
    //typedef FrBBaseObjectMap Storage;
    typedef std::map<const String, FrBVar> Storage;
    
    /** Contient des instances */
    Storage _data;

    
    /** _collect_threshold est un nombre de FrBVar */ 
    size_t _collect_threshold;
    
public:
   
    inline FrBMemory() : _collect_threshold(1 * MB / BLOCK_SIZE) {};
    
    ~FrBMemory();
    
public:


    /** collect_threshold est en octet */
    inline void setCollectThreshold(size_t s) { _collect_threshold = s / BLOCK_SIZE + 1; }
    
    //TODO verifier l'existance
    FrBBaseObject* addObject(const String& name, const FrBClass * type, FrBBaseObject* obj)
    {
        /* en cas de succès */
        
        if(_data.size() >= _collect_threshold)
            collect();
        
        _data[name] = FrBVar(type, obj);
        return obj;
    }
    
    //TODO void deleteObject(FrBBaseObject)
    void deleteObject(FrBBaseObject* obj)
    {
        /* en cas de succès */
    }
    
    size_t collect(int pass = 1)
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
    
    /** taille en octet */
    inline size_t size() const { return _data.size() * BLOCK_SIZE; }

    /** taille en block */
    inline size_t bsize() const { return _data.size(); }
        
    inline FrBBaseObject* getObject(const String& name)
    {
        Storage::iterator it = _data.find(name);
        
        return (it == _data.end()) ? 0 : it->second.value;
    }
    
    
    std::ostream& print(int cols = 5, std::ostream& out = std::cout) const;
};

#endif

