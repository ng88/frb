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




#include "frbinterpreter.h"
#include <iostream>

#define debug(x) { DebugInfos db = { "", x }; EMIT_EVENT(Debug, db); }
#define error(l, x, c) { ErrorInfos err = { l, 0, 0, x, c }; EMIT_EVENT(Error, err); }

FrBInterpreter::FrBInterpreter()
{
    CONNECT_EVENT( Error, NULL );
    CONNECT_EVENT( Debug, NULL );
    
}

FrBInterpreter::~FrBInterpreter()
{
}    

void FrBInterpreter::addCode(const String& code)
{
    _code += code;
}

void FrBInterpreter::addObject(FrBBaseObject * obj)
{
    FRB_UNUSED(obj);
}




bool FrBInterpreter::execute()
{   
    
    bool ret = _parser.parse(_code);
    
    return ret;
}

bool FrBInterpreter::execute(const String& code, ErrorCallback fn)
{
    FrBInterpreter tmp;
    tmp.addCode(code);
    tmp.CONNECT_EVENT(Error, fn);
    return tmp.execute();
}



