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


#ifndef FRBLEXER_H
#define FRBLEXER_H

#include "../common/string.h"

class FrBSyntaxicalParser;
class FrBErrorCollector;


class FrBFlexLexerEx : public FrBFlexLexer
{
private:
    /* set de mot clef utilisé */
    int keyword_set; /* set 0 pris par défaut */
    bool first_instruction;
    String _current_file;
    
    
public:
    FrBFlexLexerEx()
    : keyword_set(0), first_instruction(true) {}
    
    FrBSyntaxicalParser * frb_syntaxer;
    FrBErrorCollector * frb_error;
    
    virtual int yylex();
    String& currentFile() { return _current_file; }
};

//typedef FrBFlexLexer FrBFlexLexerEx;

#endif
