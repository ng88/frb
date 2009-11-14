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



#include "string.h"



bool StringEx::isSpacesOnly(const String& str)
{
    for( String::const_iterator it = str.begin(); it != str.end(); ++it )
        if( !isspace(*it) )
            return false;
    
    return true;
}

String StringEx::ltrim(const String& str)
{
    for( String::const_iterator it = str.begin(); it != str.end(); ++it )
        if( !isspace(*it) )
            return String(it, str.end());
        
    return str;
}

String StringEx::rtrim(const String& str)
{
    for( String::const_reverse_iterator it = str.rbegin(); it != str.rend(); ++it )
        if( !isspace(*it) )
            return String(str.begin(), it.base());
        
    return str;
}

String StringEx::trim(const String& str)
{
    String::const_iterator start = str.begin();
    for( String::const_iterator it = str.begin(); it != str.end(); ++it )
        if( !isspace(*it) )
        {
            start = it;
            break;
        }
    
    String::const_iterator end = str.end();    
    for( String::const_reverse_iterator it = str.rbegin(); it != str.rend(); ++it )
        if( !isspace(*it) )
        {
            end = it.base();
            break;
        }
        
    return String(start, end);
        
}

