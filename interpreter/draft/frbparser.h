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


#ifndef FRBPARSER_H
#define FRBPARSER_H

#include "string.h"

#include <vector>

#include "frbparsingtree.h"

#undef yyFlexLexer
#define yyFlexLexer FrBFlexLexer
#include <FlexLexer.h>


#include "syntaxer.h"   //syntaxical


class FrBParser
{
private:
    FrBParseNode * _base; /* parsing tree */
    
    FrBSyntaxicalParser _syntaxer;

public:
    bool parse(const String& str);

};

#endif
