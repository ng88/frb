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
    
    FrBFlexLexerEx * _lexer; /* lexer */
    FrBSyntaxicalParser * _syntaxer; /* syntaxer */
    FrBErrorCollector _errors; /* error list */
    
    bool _disposed; /* true if mem and parsing tree have been disposed */
    
    void dispose_classes(FrBClassList* lst);

public:
    
    FrBParser();
    ~FrBParser();
    
    inline Tree * classList() { return &_classes; }
    
    void dispose();
    
    bool parse(const String& in);
    bool parse(std::istream& in_stream = std::cin);
    
    std::ostream& printTree(std::ostream& out = std::cout) const;

};

#endif
