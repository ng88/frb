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



#ifndef FRBINTERPRETER_H
#define FRBINTERPRETER_H

#include "frbmisc.h"
#include "frbtypes.h"
#include "events.h"
#include "frbmem.h"
#include "frbparser.h"


enum ParseRet
{
    prNext = 0, /* use next function for parsing */
    prOk = 1, /* parsing ok with this function */
    prErr = 2 /* error while parsing with this function */
};


class FrBInterpreter
{
private:
    
    FrBParser _parser;
    FrBMemManager _mem;
    
    String _code;
    
    ParseRet process_line(const String& line);
    
    ParseRet process_declare(const String& line);
    
public:

    DEC_EVENT(Error);
    DEC_EVENT(Debug);
    
    FrBInterpreter();
    ~FrBInterpreter();
    
    void addCode(const String& code);
    void addObject(FrBBaseObject * obj);
    
    bool execute();
    
    static bool execute(const String& code, ErrorCallback fn = NULL);
};

#endif
