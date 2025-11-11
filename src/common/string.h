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



#ifndef STRING_H
#define STRING_H

#include <sstream>

#include <string>
#include <vector>
#include <stack>

#include <cctype>
#include <cstring>

/** Alias for std::string (to make a potential change easy) */ 
typedef std::string String;

/** Alias for char* (to make a potential change easy) */
#define CString char*
//typedef char* CString;

/** Vector of String */
typedef std::vector<String> StringList;

/** Vector of CString */
typedef std::vector<char*> CStringList;

/** Stack of String */
typedef std::stack<String> StringStack;

/** Stack of CString */
typedef std::stack<char*> CStringStack;

/** Static class that provides extended functions on String */
class StringEx 
{
public:

    /** Compute the size of 'str', used in split */
    static inline size_t str_size(const CString& str) { return strlen(str); }

    /** Compute the size of 'str', used in split */
    static inline size_t str_size(const String& str) { return str.size(); }

    /** Compute the size of 'str', used in split */
    static inline size_t str_size(const char& str) { return 1; }

    /** Split 'source' in 'container' using 'delim' as delimiter.
      * 'delim' can be a char, a C string or a String
      * Return the number of token added to container
      */
    template<class T, class D>
    static size_t split(T &container, const String& source, const D& delim = '\n', bool allowEmptyEntries = true)
    {
        size_t pos = std::string::npos;
        size_t old_pos = 0;
        size_t delim_size = str_size(delim);
        size_t source_size = str_size(source);
        size_t found = 0;
        
        if(delim_size == 0 || source_size == 0)
            return 0;
        
        while( (pos = source.find(delim, old_pos)) != std::string::npos )
        {
            if( allowEmptyEntries || (!allowEmptyEntries && pos != old_pos) )
            {
                container.push_back( source.substr(old_pos, pos - old_pos) );
                found++;
            }
               
            old_pos = pos + delim_size;
        }
        
        if( allowEmptyEntries || (!allowEmptyEntries && source_size != old_pos) )
        {
            container.push_back( source.substr(old_pos, source_size - old_pos) );
            found++;
        }
        
        return found;
                
    }
    
    /** Return true if 'str' is only made of spaces
        (the C function isspace() is used to determine if a char is a space) */
    static bool isSpacesOnly(const String& str);
    
    /** Return the 'n' chars at left of 'str' */
    static inline String left(const String& str, const String::size_type& pos)
    {
        return String( str, 0, pos );
    }
    
    /** Return the 'n' chars at right of 'str' */
    static inline String right(const String& str, const String::size_type& pos)
    {
        return String( str, str.length() - 1 - pos, pos );
    }
    
    /** Return true if 'c' is a space 
        (this is an alias for the C function isspace()) */
    static inline bool isSpace(const char& c)
    {
        return isspace(c);
    }
    
    static String ltrim(const String& str);
    static String rtrim(const String& str);
    static String trim(const String& str);
    
    
    template<class T>
    static inline String nb2string(T v)
    {
        std::ostringstream tmp;
        tmp << v;
        return tmp.str();
    }
    
    static inline String int2string(int v)
    {
        return nb2string(v);
    }
    
    static inline String double2string(double v)
    {
        return nb2string(v);
    }

};


#endif
