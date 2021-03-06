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


