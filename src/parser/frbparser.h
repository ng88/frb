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


#ifndef FRBPARSER_H
#define FRBPARSER_H

#include "../common/string.h"

#include <vector>

#include <iostream>


#undef yyFlexLexer //lexical parser
#define yyFlexLexer FrBFlexLexer
#include <FlexLexer.h>

#include "frblexer.h"

#include "frbsyntaxer.h"
#include "syntaxer.h"
#include "frbtemplatepool.h"

/*

Class FrBParser
{
    FrBFlexLexer * _lexer
    FrBSyntaxicalParser * _syntaxer
    
}

Class FrBSyntaxicalParser
    FrBSyntaxicalParser(FrBFlexLexer*)
    FrBFlexLexer * frb_lexer


Class FrBFlexLexer
    FrBSyntaxicalParser * frb_syntaxer
    
*/

//TODO: retourner l'error collector

class FrBErrorCollector;

class FrBParser
{
public:
    typedef FrBClassMap Tree;
    
private:
    Tree _classes; /* parsing tree */
    FrBTemplatePool  _templatePool;
    
    FrBFlexLexerEx * _lexer; /* lexer */
    FrBSyntaxicalParser * _syntaxer; /* syntaxer */
    FrBErrorCollector _errors; /* error list */
    
    bool _disposed; /* true if mem and parsing tree have been disposed */
    
    void dispose_classes(FrBClassList* lst);

public:
    
    FrBParser();
    ~FrBParser();
    
    inline Tree * classList() { return &_classes; }
    inline FrBTemplatePool * templatePool() { return &_templatePool; }
    
    void dispose();
    
    bool parse(const String& in);
    bool parse(std::istream& in_stream = std::cin);
    
    void resolveAndCheckTree(FrBResolveEnvironment&) throw (FrBResolveException);
    
    std::ostream& printTree(std::ostream& out = std::cout, bool user_class_only = false) const;

};

#endif
