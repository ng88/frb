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




#include "frbparser.h"
#include <iostream>
#include <sstream>
#include "../common/assert.h"

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

void FrBParser::resolveAndCheckTree(FrBResolveEnvironment& e) throw (FrBResolveException)
{ 

    for(Tree::const_iterator it = _classes.begin(); it != _classes.end(); ++it)
    {
        frb_assert(it->second);
        it->second->resolvePrototype(e);  
    }

    for(Tree::const_iterator it = _classes.begin(); it != _classes.end(); ++it)
    {
        frb_assert(it->second);
        it->second->resolveAndCheck(e);  
    }
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

std::ostream& FrBParser::printTree(std::ostream& sout, bool user_class_only) const
{

    for(Tree::const_iterator it = _classes.begin(); it != _classes.end(); ++it)
    {
        FrBClass * c = it->second;
        
        if(user_class_only)
        {
            if(dynamic_cast<FrBCodeClass*>(c))
                sout << *c;
        }
        else
            sout << *c;
        
    }

    return sout;
}


