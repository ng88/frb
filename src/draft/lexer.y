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




%{

#include "frbkeywords.h"
#include "syntaxer.h"
#include "frbparsingtree.h"

#define YYSTYPE FrBParseNode*
extern YYSTYPE yylval;

//#define TOKEN(x) FrBSyntaxicalParser::x

#define TOKEN(x) (printf("Token : " #x " %s \n", yytext), FrBSyntaxicalParser::x)
#define GET_KEYWORD(x) FrBKeywords::findKeyword(x)
#define KEYWORD(x) FrBKeywords::x

#include <cstdlib>

%}


%pointer
%option noyywrap
%option yylineno
%option c++

frb_blank             [ \t\v]
frb_blanks            ({frb_blank}+)
frb_blanks_nl         {frb_blanks}|"\n"
frb_newline           [\n\r]
frb_digit             [0-9]
frb_integer           {frb_digit}+
frb_float             {frb_integer}("."{frb_integer})?([eE][+-]?{frb_integer})?
frb_identifier        [_a-zA-Z]([_a-zA-Z0-9]*)
frb_keyword           [A-Z]([a-zA-Z]*)
frb_array_identifier  [_a-zA-Z]([_a-zA-Z0-9]*)("("{frb_integer}(","{frb_integer})*")")


%x frb_ccd_declare

%%
{frb_blank}*                        {}

{frb_keyword}                       { /* mot clef ou id */
                                       if( yytext[yyleng-1] == '\n' )
                                          unput('\n');
                                       printf("yytext=%s, GET_KEYWORD(yytext)=%d\n", yytext, GET_KEYWORD(yytext));
                                       switch(GET_KEYWORD(yytext))
                                       {
                                        case KEYWORD(FRB_KW__NOTFOUND):
                                            return TOKEN(FRB_IDENTIFIER);
                                        case KEYWORD(FRB_KW_DECLARE):
                                            return TOKEN(FRB_STAT_DECLARE);
                                        case KEYWORD(FRB_KW_DECLAREAS):
                                            return TOKEN(FRB_STAT_DECLAREAS);
                                       }
                                       
                                       
                                    }

{frb_identifier}                    {
                                       if( yytext[yyleng-1] == '\n' )
                                          unput('\n');
                                          
                                       return TOKEN(FRB_IDENTIFIER);
                                    }


{frb_newline}        { BEGIN(0); return TOKEN(FRB_END_LINE); }


{frb_integer}        { return TOKEN(FRB_TYPE_BUILTIN_INT); }
{frb_float}          { return TOKEN(FRB_TYPE_BUILTIN_DOUBLE); }


<<EOF>>    { return TOKEN(FRB_END_OF_FILE); }

. { printf("Inconnu : %s\n", yytext ); }



