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



%{

#undef yyFlexLexer //lexical parser
#define yyFlexLexer FrBFlexLexer
#include <FlexLexer.h>


#include "frbparsingtree.h"

#define YYSTYPE FrBParseNode*
extern YYSTYPE yylval;

#include <cstdio>

    
%}

%name FrBSyntaxicalParser

%start list

%token FRB_TYPE_BUILTIN_DOUBLE FRB_TYPE_BUILTIN_INT
%token FRB_TYPE_BUILTIN_SRING FRB_TYPE_BUILTIN_CHAR

%token FRB_OP_PLUS FRB_OP_MINUS
%token FRB_OP_DIVIDE FRB_OP_MULT
%token FRB_OP_POW
%token FRB_OP_LPAR FRB_OP_RPAR

%token FRB_END_LINE
%token FRB_END_OF_FILE

%token FRB_STAT_DECLARE FRB_STAT_DECLAREAS
%token FRB_IDENTIFIER

%left FRB_OP_PLUS FRB_OP_MINUS
%left FRB_OP_DIVIDE FRB_OP_MULT
%left FRB_OP_UNARYMINUS
%right FRB_OP_POW


%%

list :
     | list stat FRB_END_LINE
     | list dec FRB_END_LINE
     | FRB_END_LINE
     | list error FRB_END_LINE
              { yyerrok; }
     ;

stat : expr {  }
     | dec  {  }

expr : FRB_OP_LPAR expr FRB_OP_RPAR { $$ = $2; }
     | FRB_TYPE_BUILTIN_INT         { $$ = $1; }
     ;

dec : FRB_STAT_DECLARE dec_as_list { /* prise en charge complete du Dim ... */ }
    ;
    
dec_as_list : dec_as_list  dec_id_list FRB_STAT_DECLAREAS FRB_IDENTIFIER
            | dec_as_list  FRB_IDENTIFIER FRB_STAT_DECLAREAS FRB_IDENTIFIER  expr
            | dec_id_list FRB_STAT_DECLAREAS FRB_IDENTIFIER { /* id, id, ... As id */ }
            | FRB_IDENTIFIER FRB_STAT_DECLAREAS FRB_IDENTIFIER  expr { /* id As id = expr*/ }
            ;

dec_id_list : dec_id_list  FRB_IDENTIFIER { /* id, id, ... */ }
            | FRB_IDENTIFIER
            ;
        





