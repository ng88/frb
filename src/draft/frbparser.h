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
