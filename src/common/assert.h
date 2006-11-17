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




/** check that cond is true, str is displayed if it failed */
#define frb_assert2(cond, str) __assert((cond), __LINE__, __FILE__, (str), #cond)

/** check that cond is true, "assertion failed" is displayed if it failed */
#define frb_assert(cond) frb_assert2((cond), "assertion failed")


/** warm by showing str if cond is not true */
#define frb_warning2(cond, str) __warning((cond), __LINE__, __FILE__, (str), #cond)

/** warm by showing "warning" if cond is not true */
#define frb_warning(cond) frb_warning2((cond), "warning")

/** define code that will be used only when assertions are used */
#define FRB_ASSERT_CODE(code) __FRB_ASSERT_CODE(code)






#ifdef DEBUG_ASSERT

inline void __assert(bool cond, int line, char * file, char * str, char * cond_str)
{
    using namespace std;
    
    if(!(cond))
    {
        cerr.flush();
        cout.flush();
        cerr << "assertion error on `" << cond_str << "' in " << file
             << " at line " << line  << ": " << str << ", should we continue (y/n)? ";

	cerr.flush();

        string str;
        cin >> str;
        
        if(str[0] != 'y' && str[0] != 'Y') abort();
    }
}

inline void __warning(bool cond, int line, char * file, char * str, char * cond_str)
{
    if(!(cond))
    {
      std::cerr.flush();
      std::cout.flush();
      std::cerr << "warning on `" << cond_str << "' in " << file
		<< " at line " << line  << ": " << str << std::endl;
    }
}

#define __FRB_ASSERT_CODE(code) code

#else

#define __assert(cond, line, file, str, cond_str)
#define __warning(cond, line, file, str, cond_str)
#define __FRB_ASSERT_CODE(code)


#endif

#ifndef __FILE__
# define __FILE__ "__FILE__ not defined"
#endif

#ifndef __LINE__
# define __LINE__ -1
#endif



#endif
