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

#include "../common/assert.h"

class FrBClass;

class FrBMember /* a frb member */
{
protected:
    FrBClass* _contClass;
    
public:

    FRB_ASSERT_CODE(FrBMember() : _contClass(0) {});
    
    inline void setContainer(FrBClass* c);
    inline const FrBClass* container() const;
    inline FrBClass* containerPtr();
        
};






















/*                   inlined                   */


inline void FrBMember::setContainer(FrBClass* c)
{
    frb_assert(c);
    _contClass = c;
}

inline const FrBClass* FrBMember::container() const
{
    frb_warning(_contClass);
    return _contClass;
}

inline FrBClass* FrBMember::containerPtr()
{
    frb_warning(_contClass);
    return _contClass;
}

#endif


