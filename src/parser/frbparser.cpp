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




#include "frbparser.h"
#include <iostream>
#include <sstream>
#include "../common/assert.h"
/*
  this include is used by the get_dependencies.sh script
  #include "parser"

*/

using namespace std;

FrBParser::FrBParser()
 : _disposed(true)
{
    _lexer = new FrBFlexLexerEx();
    _syntaxer = new FrBSyntaxicalParser();
    
    _syntaxer->frb_lexer = _lexer;
    _syntaxer->frb_classes = &_classes;
    _syntaxer->templatePool = &_templatePool;
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
    dispose();
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
    if(_disposed)
        return;

    //_templatePool.createInstances(e);

    for(Tree::const_iterator it = _classes.begin(); it != _classes.end(); ++it)
    {
        frb_assert(it->second);
	std::cout << "resolvproto de " << it->first << "\n";
	if(!it->second->isATemplate())
	    it->second->resolvePrototype(e);
    }

    for(Tree::const_iterator it = _classes.begin(); it != _classes.end(); ++it)
    {
	std::cout << "resolvcheck de " << it->first << "\n";
	if(!it->second->isATemplate() && it->first[it->first.size() - 1] != '$')
	    it->second->resolveAndCheck(e);
    }

    //_templatePool.resolveAndCheckCreatedClasses(e);
}

void FrBParser::dispose()
{
    if(_disposed)
        return;
    
    // destuct parsing tree
    for(Tree::iterator it = _classes.begin(); it != _classes.end(); ++it)
    {

        if(it->second->name() == it->first) // we delete only non-alias    
            delete it->second;

    }
        
    _classes.clear();
        
    _disposed = true;
    
}

std::ostream& FrBParser::printTree(std::ostream& sout, bool user_class_only) const
{
    if(_disposed)
        return sout;

    for(Tree::const_iterator it = _classes.begin(); it != _classes.end(); ++it)
    {
        FrBClass * c = it->second;
        
        if(!user_class_only || (user_class_only && dynamic_cast<FrBCodeClass*>(c) ))
        {
            if(c->name() == it->first)
		sout << *c;
	    else if(it->first[it->first.size() - 1] == '$')
                sout << "' " << it->first << " is a templace instance of " << c->name() << endl << *c;
            else
                sout << "' " << it->first << " is an alias for " << c->name() << endl;                
        }
        
    }

    return sout;
}


