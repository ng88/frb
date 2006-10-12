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



#include <iostream>
#include "frbinterpreter.h"

void script_error( PARAM_FN_EVENT(Error) e )
{
    fprintf(stderr, "\tLine: %d\tCol: %d\tErrID: %d\tError: %s\tCode: %s\n",
            e.line, e.col, e.errid, e.errstr.c_str(), e.code.c_str());
}

void frb_debug( PARAM_FN_EVENT(Debug) e )
{
    fprintf(stderr, "\tCaller: %s\tDebug Str: %s\n",
            e.caller.c_str(), e.debugstr.c_str());
}

int main(int argc, char ** argv)
{

    FrBInterpreter test;
    test.CONNECT_EVENT( Error, script_error );
    test.CONNECT_EVENT( Debug, frb_debug );
    
    test.addCode( 
                    /*"Dim q As Int\n"
                    "Dim i As Int = 78\n"
                    "Dim j As Int, k As String\n"
                    "Dim a, b, c As String, console As Console\n"
                    "Dim z As Int = 3\n"
                    "Dim kl, io As String, iiii As Int, ooo As String = \"salut\"\n"*/
                    /*"1+1*8+(8*7*(2+(1+(0+1 * 45))))\n"*/
                    "((3))\n"
               );
                
    test.execute();
    
    
    return 0;

}
