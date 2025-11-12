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




#include "frbkeywords.h"
#include <cstring>
#include <cstdio>

char* FrBKeywords::key[FRB_KW__COUNT] = { "Dim", "As" };


int FrBKeywords::findKeyword(char* str)
{
    for(int i = 0; i <  FRB_KW__COUNT; ++i)
        if( !strcmp(key[i], str) )
            return i;
            
    return FRB_KW__NOTFOUND;
}



