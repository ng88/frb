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



#ifndef FRBKEYWORDS_H
#define FRBKEYWORDS_H


class FrBKeywords
{
public:
    enum 
    {
        FRB_KW__NOTFOUND = -1,
        FRB_KW_DECLARE = 0,
        FRB_KW_DECLAREAS,
        FRB_KW_CONST,
        FRB_KW_STATIC,
        FRB_KW_PUBLIC,
        FRB_KW_PRIVATE,
        FRB_KW_PROTECTED,
        
        FRB_KW__COUNT
    };
    
    static char* key[FRB_KW__COUNT];
    static int findKeyword(char* str);
};

#endif
