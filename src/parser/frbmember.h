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


#ifndef FRBMEMBER_H
#define FRBMEMBER_H


class FrBClass;

class FrBMember /* a frb member */
{
protected:
    const FrBClass* _contClass;
    
public:
    
    inline void setContainer(const FrBClass* c) { _contClass = c; }
    inline const FrBClass* container() const { return _contClass; }    
        
};


#endif


