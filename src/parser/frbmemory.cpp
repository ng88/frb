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


FrBMemory * FrBMemory::_mem = new FrBMemory();

FrBMemory::~FrBMemory()
{

    for(Storage::iterator it = _data.begin(); it != _data.end(); ++it)
        ;//delete it->second.value;
        
    frb_warning2(false, "memory not disposed");
}



std::ostream& FrBMemory::print(int cols, std::ostream& out) const
{
    int nb = 1;
    
    out << "Memory size: " << size() << " bytes / " << bsize() << " blocks\n";
    out << "             (1 block = " << BLOCK_SIZE << " bytes)\n";
    
    for(Storage::const_iterator it = _data.begin(); it != _data.end(); ++it)
    {
        out << '[' <<  it->first << '@' << reinterpret_cast<void*>(it->second.value)
                         << ':' << it->second.type->typeID() << '('
                         << it->second.type->name() << "), l" << it->second.links << "]\t";
        nb++;
        
        if(nb % cols == 0) out << '\n';
    }
    
    if(nb % cols != 0) out << '\n';
    
    return out;
}
