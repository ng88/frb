

#include "frbparser.h"
#include <iostream>
#include <sstream>

using namespace std;

bool FrBParser::parse(const String& str)
{
    //ostringstream out;
    //istringstream in(str);
    
    _syntaxer.switch_lexer_streams( &cin, &cout);
    _syntaxer.yyparse();
    
    return true;
}


