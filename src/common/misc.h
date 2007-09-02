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



#ifndef MISC_H
#define MISC_H

#include <typeinfo>
#include <cstring>
#include <cstdlib>

#define copy_not_null(c) (c) ? (c) : ((c) = Misc::copy(this))

/** Miscellaneous functions */
class Misc
{
public:

    /** Return true if b is a kind of A 
      * Example : if(Misc::isKindOf<MyClass>(var)) { ... var is an instance of a class that inherits of MyClass ... }
      */
    template<class A, class B>
    static inline bool isKindOf(B * b)
    {
	return dynamic_cast<A*>(b) != 0;
    }

    /** Return true if b is an intance of A 
      * Example : if(Misc::isTypeOf<MyClass>(var)) { ... var is an instance of MyClass ... }
      */
    template<class A, class B>
    static inline bool isTypeOf(B * b)
    {
	return typeid(b) == typeid(A);
    }

    template<class A>
    inline static A * copy(A * c)
    {
	return (A*)memcpy(malloc(sizeof(A)), c, sizeof(A));
    }

    template<class A>
    inline static A * copy(const A * c)
    {
	return copy(const_cast<A*>(c));
    }


};

#endif
