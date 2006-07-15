#ifndef FRBLEXER_H
#define FRBLEXER_H

#include "../common/string.h"

class FrBSyntaxicalParser;
class FrBErrorCollector;


class FrBFlexLexerEx : public FrBFlexLexer
{
private:
    /* set de mot clef utilis� */
    int keyword_set; /* set 0 pris par d�faut */
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
