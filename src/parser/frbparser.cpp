

#include "frbparser.h"
#include <iostream>
#include <sstream>

using namespace std;

FrBParser::FrBParser()
 : _disposed(true)
{
    _lexer = new FrBFlexLexerEx();
    _syntaxer = new FrBSyntaxicalParser();
    
    _syntaxer->frb_lexer = _lexer;
    _syntaxer->frb_classes = &_classes;
    _syntaxer->frb_error = &_errors;
    
    _lexer->frb_syntaxer = _syntaxer;
    _lexer->frb_error = &_errors;
    
    FrBClass::root = &_classes;

}

FrBParser::~FrBParser()
{
    dispose();
    delete _lexer;
    delete _syntaxer;
}

bool FrBParser::parse(istream& in_stream)
{
    _disposed = false;

    _syntaxer->switch_lexer_streams( &in_stream, &cerr );
    
    return _syntaxer->yyparse() == 0;
}

bool FrBParser::parse(const String& str)
{
    istringstream in(str);

    return parse(in);
}

void FrBParser::dispose()
{
    if(_disposed)
        return;
    
    // destuct parsing tree
    for(Tree::iterator it = _classes.begin(); it != _classes.end(); ++it) 
        ;//delete it->second;
     
    frb_warning2(false, "tree not disposed");
        
    _classes.clear();
        
    _disposed = true;
    
}

std::ostream& FrBParser::printTree(std::ostream& sout) const
{

    for(Tree::const_iterator it = _classes.begin(); it != _classes.end(); ++it)
        sout << *(it->second);

    return sout;
}


