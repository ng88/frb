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


#ifndef FRB_ASSERT_H
#define FRB_ASSERT_H

#include <iostream>

#define DEBUG_ASSERT

#ifdef DEBUG_ASSERT

inline void __assert(bool cond, int line, char * file, char * str, char * cond_str)
{
    using namespace std;
    
    if(!(cond))
    {
        cerr << "assertion error on `" << cond_str << "' in " << file
             << " at line " << line  << ": " << str << ", should we continue (y/n)? ";

        string str;
        cin >> str;
        
        if(str[0] != 'y' && str[0] != 'Y') exit(1);
    }
}

inline void __warning(bool cond, int line, char * file, char * str, char * cond_str)
{
    if(!(cond))
    {
        std::cerr << "warning on `" << cond_str << "' in " << file
                    << " at line " << line  << ": " << str << std::endl;
    }
}

#else

#define __assert(cond, line, file, str, cond_str)
#define __warning(cond, line, file, str, cond_str)

#endif


#define frb_assert2(cond, str) __assert((cond), __LINE__, __FILE__, (str), #cond)
#define frb_assert(cond) frb_assert2((cond), "assertion failed")

#define frb_warning2(cond, str) __warning((cond), __LINE__, __FILE__, (str), #cond)
#define frb_warning(cond) frb_warning2((cond), "warning")

#endif
