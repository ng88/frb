#define YY_FrBSyntaxicalParser_h_included

/*  A Bison++ parser, made from syntaxer.y  */

 /* with Bison++ version bison++ Version 1.21-8, adapted from GNU bison by coetmeur@icdc.fr
  */


#line 1 "/usr/lib/bison.cc"
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Bob Corbett and Richard Stallman

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* HEADER SECTION */
#if defined( _MSDOS ) || defined(MSDOS) || defined(__MSDOS__) 
#define __MSDOS_AND_ALIKE
#endif
#if defined(_WINDOWS) && defined(_MSC_VER)
#define __HAVE_NO_ALLOCA
#define __MSDOS_AND_ALIKE
#endif

#ifndef alloca
#if defined( __GNUC__)
#define alloca __builtin_alloca

#elif (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc)  || defined (__sgi)
#include <alloca.h>

#elif defined (__MSDOS_AND_ALIKE)
#include <malloc.h>
#ifndef __TURBOC__
/* MS C runtime lib */
#define alloca _alloca
#endif

#elif defined(_AIX)
#include <malloc.h>
#pragma alloca

#elif defined(__hpux)
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */

#endif /* not _AIX  not MSDOS, or __TURBOC__ or _AIX, not sparc.  */
#endif /* alloca not defined.  */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif
#ifdef __cplusplus
#ifndef YY_USE_CLASS
#define YY_USE_CLASS
#endif
#else
#ifndef __STDC__
#define const
#endif
#endif
#include <stdio.h>
#define YYBISON 1  

/* #line 73 "/usr/lib/bison.cc" */
#line 85 "syntaxer.tab.c"
#line 2 "syntaxer.y"


#undef yyFlexLexer //lexical parser
#define yyFlexLexer FrBFlexLexer
#include <FlexLexer.h>


#include "frbparsingtree.h"

#define YYSTYPE FrBParseNode*
extern YYSTYPE yylval;

#include <cstdio>

    

#line 73 "/usr/lib/bison.cc"
/* %{ and %header{ and %union, during decl */
#define YY_FrBSyntaxicalParser_BISON 1
#ifndef YY_FrBSyntaxicalParser_COMPATIBILITY
#ifndef YY_USE_CLASS
#define  YY_FrBSyntaxicalParser_COMPATIBILITY 1
#else
#define  YY_FrBSyntaxicalParser_COMPATIBILITY 0
#endif
#endif

#if YY_FrBSyntaxicalParser_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YYLTYPE
#ifndef YY_FrBSyntaxicalParser_LTYPE
#define YY_FrBSyntaxicalParser_LTYPE YYLTYPE
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_FrBSyntaxicalParser_STYPE 
#define YY_FrBSyntaxicalParser_STYPE YYSTYPE
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_FrBSyntaxicalParser_DEBUG
#define  YY_FrBSyntaxicalParser_DEBUG YYDEBUG
#endif
#endif
#ifdef YY_FrBSyntaxicalParser_STYPE
#ifndef yystype
#define yystype YY_FrBSyntaxicalParser_STYPE
#endif
#endif
/* use goto to be compatible */
#ifndef YY_FrBSyntaxicalParser_USE_GOTO
#define YY_FrBSyntaxicalParser_USE_GOTO 1
#endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_FrBSyntaxicalParser_USE_GOTO
#define YY_FrBSyntaxicalParser_USE_GOTO 0
#endif

#ifndef YY_FrBSyntaxicalParser_PURE

/* #line 117 "/usr/lib/bison.cc" */
#line 150 "syntaxer.tab.c"

#line 117 "/usr/lib/bison.cc"
/*  YY_FrBSyntaxicalParser_PURE */
#endif

/* section apres lecture def, avant lecture grammaire S2 */

/* #line 121 "/usr/lib/bison.cc" */
#line 159 "syntaxer.tab.c"

#line 121 "/usr/lib/bison.cc"
/* prefix */
#ifndef YY_FrBSyntaxicalParser_DEBUG

/* #line 123 "/usr/lib/bison.cc" */
#line 166 "syntaxer.tab.c"

#line 123 "/usr/lib/bison.cc"
/* YY_FrBSyntaxicalParser_DEBUG */
#endif


#ifndef YY_FrBSyntaxicalParser_LSP_NEEDED

/* #line 128 "/usr/lib/bison.cc" */
#line 176 "syntaxer.tab.c"

#line 128 "/usr/lib/bison.cc"
 /* YY_FrBSyntaxicalParser_LSP_NEEDED*/
#endif



/* DEFAULT LTYPE*/
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
#ifndef YY_FrBSyntaxicalParser_LTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YY_FrBSyntaxicalParser_LTYPE yyltype
#endif
#endif
/* DEFAULT STYPE*/
      /* We used to use `unsigned long' as YY_FrBSyntaxicalParser_STYPE on MSDOS,
	 but it seems better to be consistent.
	 Most programs should declare their own type anyway.  */

#ifndef YY_FrBSyntaxicalParser_STYPE
#define YY_FrBSyntaxicalParser_STYPE int
#endif
/* DEFAULT MISCELANEOUS */
#ifndef YY_FrBSyntaxicalParser_PARSE
#define YY_FrBSyntaxicalParser_PARSE yyparse
#endif
#ifndef YY_FrBSyntaxicalParser_LEX
#define YY_FrBSyntaxicalParser_LEX yylex
#endif
#ifndef YY_FrBSyntaxicalParser_LVAL
#define YY_FrBSyntaxicalParser_LVAL yylval
#endif
#ifndef YY_FrBSyntaxicalParser_LLOC
#define YY_FrBSyntaxicalParser_LLOC yylloc
#endif
#ifndef YY_FrBSyntaxicalParser_CHAR
#define YY_FrBSyntaxicalParser_CHAR yychar
#endif
#ifndef YY_FrBSyntaxicalParser_NERRS
#define YY_FrBSyntaxicalParser_NERRS yynerrs
#endif
#ifndef YY_FrBSyntaxicalParser_DEBUG_FLAG
#define YY_FrBSyntaxicalParser_DEBUG_FLAG yydebug
#endif
#ifndef YY_FrBSyntaxicalParser_ERROR
#define YY_FrBSyntaxicalParser_ERROR yyerror
#endif
#ifndef YY_FrBSyntaxicalParser_PARSE_PARAM
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
#define YY_FrBSyntaxicalParser_PARSE_PARAM
#ifndef YY_FrBSyntaxicalParser_PARSE_PARAM_DEF
#define YY_FrBSyntaxicalParser_PARSE_PARAM_DEF
#endif
#endif
#endif
#endif
#ifndef YY_FrBSyntaxicalParser_PARSE_PARAM
#define YY_FrBSyntaxicalParser_PARSE_PARAM void
#endif
#endif
#if YY_FrBSyntaxicalParser_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YY_FrBSyntaxicalParser_LTYPE
#ifndef YYLTYPE
#define YYLTYPE YY_FrBSyntaxicalParser_LTYPE
#else
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
#endif
#endif
#ifndef YYSTYPE
#define YYSTYPE YY_FrBSyntaxicalParser_STYPE
#else
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
#endif
#ifdef YY_FrBSyntaxicalParser_PURE
#ifndef YYPURE
#define YYPURE YY_FrBSyntaxicalParser_PURE
#endif
#endif
#ifdef YY_FrBSyntaxicalParser_DEBUG
#ifndef YYDEBUG
#define YYDEBUG YY_FrBSyntaxicalParser_DEBUG 
#endif
#endif
#ifndef YY_FrBSyntaxicalParser_ERROR_VERBOSE
#ifdef YYERROR_VERBOSE
#define YY_FrBSyntaxicalParser_ERROR_VERBOSE YYERROR_VERBOSE
#endif
#endif
#ifndef YY_FrBSyntaxicalParser_LSP_NEEDED
#ifdef YYLSP_NEEDED
#define YY_FrBSyntaxicalParser_LSP_NEEDED YYLSP_NEEDED
#endif
#endif
#endif
#ifndef YY_USE_CLASS
/* TOKEN C */

/* #line 236 "/usr/lib/bison.cc" */
#line 289 "syntaxer.tab.c"
#define	FRB_TYPE_BUILTIN_DOUBLE	258
#define	FRB_TYPE_BUILTIN_INT	259
#define	FRB_TYPE_BUILTIN_SRING	260
#define	FRB_TYPE_BUILTIN_CHAR	261
#define	FRB_OP_PLUS	262
#define	FRB_OP_MINUS	263
#define	FRB_OP_DIVIDE	264
#define	FRB_OP_MULT	265
#define	FRB_OP_POW	266
#define	FRB_OP_LPAR	267
#define	FRB_OP_RPAR	268
#define	FRB_END_LINE	269
#define	FRB_END_OF_FILE	270
#define	FRB_STAT_DECLARE	271
#define	FRB_STAT_DECLAREAS	272
#define	FRB_IDENTIFIER	273
#define	FRB_OP_UNARYMINUS	274


#line 236 "/usr/lib/bison.cc"
 /* #defines tokens */
#else
/* CLASS */
#ifndef YY_FrBSyntaxicalParser_CLASS
#define YY_FrBSyntaxicalParser_CLASS FrBSyntaxicalParser
#endif
#ifndef YY_FrBSyntaxicalParser_INHERIT
#define YY_FrBSyntaxicalParser_INHERIT
#endif
#ifndef YY_FrBSyntaxicalParser_MEMBERS
#define YY_FrBSyntaxicalParser_MEMBERS 
#endif
#ifndef YY_FrBSyntaxicalParser_LEX_BODY
#define YY_FrBSyntaxicalParser_LEX_BODY  
#endif
#ifndef YY_FrBSyntaxicalParser_ERROR_BODY
#define YY_FrBSyntaxicalParser_ERROR_BODY  
#endif
#ifndef YY_FrBSyntaxicalParser_CONSTRUCTOR_PARAM
#define YY_FrBSyntaxicalParser_CONSTRUCTOR_PARAM
#endif
#ifndef YY_FrBSyntaxicalParser_CONSTRUCTOR_CODE
#define YY_FrBSyntaxicalParser_CONSTRUCTOR_CODE
#endif
#ifndef YY_FrBSyntaxicalParser_CONSTRUCTOR_INIT
#define YY_FrBSyntaxicalParser_CONSTRUCTOR_INIT
#endif
/* choose between enum and const */
#ifndef YY_FrBSyntaxicalParser_USE_CONST_TOKEN
#define YY_FrBSyntaxicalParser_USE_CONST_TOKEN 0
/* yes enum is more compatible with flex,  */
/* so by default we use it */ 
#endif
#if YY_FrBSyntaxicalParser_USE_CONST_TOKEN != 0
#ifndef YY_FrBSyntaxicalParser_ENUM_TOKEN
#define YY_FrBSyntaxicalParser_ENUM_TOKEN yy_FrBSyntaxicalParser_enum_token
#endif
#endif

class YY_FrBSyntaxicalParser_CLASS YY_FrBSyntaxicalParser_INHERIT
{
public: 
#if YY_FrBSyntaxicalParser_USE_CONST_TOKEN != 0
/* static const int token ... */

/* #line 280 "/usr/lib/bison.cc" */
#line 356 "syntaxer.tab.c"
static const int FRB_TYPE_BUILTIN_DOUBLE;
static const int FRB_TYPE_BUILTIN_INT;
static const int FRB_TYPE_BUILTIN_SRING;
static const int FRB_TYPE_BUILTIN_CHAR;
static const int FRB_OP_PLUS;
static const int FRB_OP_MINUS;
static const int FRB_OP_DIVIDE;
static const int FRB_OP_MULT;
static const int FRB_OP_POW;
static const int FRB_OP_LPAR;
static const int FRB_OP_RPAR;
static const int FRB_END_LINE;
static const int FRB_END_OF_FILE;
static const int FRB_STAT_DECLARE;
static const int FRB_STAT_DECLAREAS;
static const int FRB_IDENTIFIER;
static const int FRB_OP_UNARYMINUS;


#line 280 "/usr/lib/bison.cc"
 /* decl const */
#else
enum YY_FrBSyntaxicalParser_ENUM_TOKEN { YY_FrBSyntaxicalParser_NULL_TOKEN=0

/* #line 283 "/usr/lib/bison.cc" */
#line 382 "syntaxer.tab.c"
	,FRB_TYPE_BUILTIN_DOUBLE=258
	,FRB_TYPE_BUILTIN_INT=259
	,FRB_TYPE_BUILTIN_SRING=260
	,FRB_TYPE_BUILTIN_CHAR=261
	,FRB_OP_PLUS=262
	,FRB_OP_MINUS=263
	,FRB_OP_DIVIDE=264
	,FRB_OP_MULT=265
	,FRB_OP_POW=266
	,FRB_OP_LPAR=267
	,FRB_OP_RPAR=268
	,FRB_END_LINE=269
	,FRB_END_OF_FILE=270
	,FRB_STAT_DECLARE=271
	,FRB_STAT_DECLAREAS=272
	,FRB_IDENTIFIER=273
	,FRB_OP_UNARYMINUS=274


#line 283 "/usr/lib/bison.cc"
 /* enum token */
     }; /* end of enum declaration */
#endif
public:
 int YY_FrBSyntaxicalParser_PARSE (YY_FrBSyntaxicalParser_PARSE_PARAM);
 virtual void YY_FrBSyntaxicalParser_ERROR(char *msg) YY_FrBSyntaxicalParser_ERROR_BODY;
#ifdef YY_FrBSyntaxicalParser_PURE
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
 virtual int  YY_FrBSyntaxicalParser_LEX (YY_FrBSyntaxicalParser_STYPE *YY_FrBSyntaxicalParser_LVAL,YY_FrBSyntaxicalParser_LTYPE *YY_FrBSyntaxicalParser_LLOC) YY_FrBSyntaxicalParser_LEX_BODY;
#else
 virtual int  YY_FrBSyntaxicalParser_LEX (YY_FrBSyntaxicalParser_STYPE *YY_FrBSyntaxicalParser_LVAL) YY_FrBSyntaxicalParser_LEX_BODY;
#endif
#else
 virtual int YY_FrBSyntaxicalParser_LEX() YY_FrBSyntaxicalParser_LEX_BODY;
 YY_FrBSyntaxicalParser_STYPE YY_FrBSyntaxicalParser_LVAL;
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
 YY_FrBSyntaxicalParser_LTYPE YY_FrBSyntaxicalParser_LLOC;
#endif
 int   YY_FrBSyntaxicalParser_NERRS;
 int    YY_FrBSyntaxicalParser_CHAR;
#endif
#if YY_FrBSyntaxicalParser_DEBUG != 0
 int YY_FrBSyntaxicalParser_DEBUG_FLAG;   /*  nonzero means print parse trace     */
#endif
public:
 YY_FrBSyntaxicalParser_CLASS(YY_FrBSyntaxicalParser_CONSTRUCTOR_PARAM);
public:
 YY_FrBSyntaxicalParser_MEMBERS 
};
/* other declare folow */
#if YY_FrBSyntaxicalParser_USE_CONST_TOKEN != 0

/* #line 314 "/usr/lib/bison.cc" */
#line 436 "syntaxer.tab.c"
const int YY_FrBSyntaxicalParser_CLASS::FRB_TYPE_BUILTIN_DOUBLE=258;
const int YY_FrBSyntaxicalParser_CLASS::FRB_TYPE_BUILTIN_INT=259;
const int YY_FrBSyntaxicalParser_CLASS::FRB_TYPE_BUILTIN_SRING=260;
const int YY_FrBSyntaxicalParser_CLASS::FRB_TYPE_BUILTIN_CHAR=261;
const int YY_FrBSyntaxicalParser_CLASS::FRB_OP_PLUS=262;
const int YY_FrBSyntaxicalParser_CLASS::FRB_OP_MINUS=263;
const int YY_FrBSyntaxicalParser_CLASS::FRB_OP_DIVIDE=264;
const int YY_FrBSyntaxicalParser_CLASS::FRB_OP_MULT=265;
const int YY_FrBSyntaxicalParser_CLASS::FRB_OP_POW=266;
const int YY_FrBSyntaxicalParser_CLASS::FRB_OP_LPAR=267;
const int YY_FrBSyntaxicalParser_CLASS::FRB_OP_RPAR=268;
const int YY_FrBSyntaxicalParser_CLASS::FRB_END_LINE=269;
const int YY_FrBSyntaxicalParser_CLASS::FRB_END_OF_FILE=270;
const int YY_FrBSyntaxicalParser_CLASS::FRB_STAT_DECLARE=271;
const int YY_FrBSyntaxicalParser_CLASS::FRB_STAT_DECLAREAS=272;
const int YY_FrBSyntaxicalParser_CLASS::FRB_IDENTIFIER=273;
const int YY_FrBSyntaxicalParser_CLASS::FRB_OP_UNARYMINUS=274;


#line 314 "/usr/lib/bison.cc"
 /* const YY_FrBSyntaxicalParser_CLASS::token */
#endif
/*apres const  */
YY_FrBSyntaxicalParser_CLASS::YY_FrBSyntaxicalParser_CLASS(YY_FrBSyntaxicalParser_CONSTRUCTOR_PARAM) YY_FrBSyntaxicalParser_CONSTRUCTOR_INIT
{
#if YY_FrBSyntaxicalParser_DEBUG != 0
YY_FrBSyntaxicalParser_DEBUG_FLAG=0;
#endif
YY_FrBSyntaxicalParser_CONSTRUCTOR_CODE;
};
#endif

/* #line 325 "/usr/lib/bison.cc" */
#line 470 "syntaxer.tab.c"


#define	YYFINAL		32
#define	YYFLAG		-32768
#define	YYNTBASE	20

#define YYTRANSLATE(x) ((unsigned)(x) <= 274 ? yytranslate[x] : 26)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19
};

#if YY_FrBSyntaxicalParser_DEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     5,     9,    11,    15,    17,    19,    23,    25,
    28,    33,    39,    43,    48,    51
};

static const short yyrhs[] = {    -1,
    20,    21,    14,     0,    20,    23,    14,     0,    14,     0,
    20,     1,    14,     0,    22,     0,    23,     0,    12,    22,
    13,     0,     4,     0,    16,    24,     0,    24,    25,    17,
    18,     0,    24,    18,    17,    18,    22,     0,    25,    17,
    18,     0,    18,    17,    18,    22,     0,    25,    18,     0,
    18,     0
};

#endif

#if YY_FrBSyntaxicalParser_DEBUG != 0
static const short yyrline[] = { 0,
    45,    46,    47,    48,    49,    53,    54,    56,    57,    60,
    63,    64,    65,    66,    69,    70
};

static const char * const yytname[] = {   "$","error","$illegal.","FRB_TYPE_BUILTIN_DOUBLE",
"FRB_TYPE_BUILTIN_INT","FRB_TYPE_BUILTIN_SRING","FRB_TYPE_BUILTIN_CHAR","FRB_OP_PLUS",
"FRB_OP_MINUS","FRB_OP_DIVIDE","FRB_OP_MULT","FRB_OP_POW","FRB_OP_LPAR","FRB_OP_RPAR",
"FRB_END_LINE","FRB_END_OF_FILE","FRB_STAT_DECLARE","FRB_STAT_DECLAREAS","FRB_IDENTIFIER",
"FRB_OP_UNARYMINUS","list","stat","expr","dec","dec_as_list","dec_id_list",""
};
#endif

static const short yyr1[] = {     0,
    20,    20,    20,    20,    20,    21,    21,    22,    22,    23,
    24,    24,    24,    24,    25,    25
};

static const short yyr2[] = {     0,
     0,     3,     3,     1,     3,     1,     1,     3,     1,     2,
     4,     5,     3,     4,     2,     1
};

static const short yydefact[] = {     1,
     4,     0,     0,     9,     0,     0,     0,     6,     0,     5,
     0,    16,    10,     0,     2,     3,     8,     0,    16,     0,
     0,    15,     0,     0,     0,    13,    14,     0,    11,    12,
     0,     0
};

static const short yydefgoto[] = {     2,
     7,     8,     9,    13,    14
};

static const short yypact[] = {   -10,
-32768,     1,    -4,-32768,    -1,    -6,     0,-32768,     2,-32768,
     6,    -2,     3,   -11,-32768,-32768,-32768,     4,     7,    -9,
     8,-32768,    -1,     9,    10,-32768,-32768,    -1,-32768,-32768,
    20,-32768
};

static const short yypgoto[] = {-32768,
-32768,    -5,-32768,-32768,    12
};


#define	YYLAST		28


static const short yytable[] = {    11,
    31,     3,     4,     1,     4,    21,    22,    25,    22,    10,
     5,    12,     5,    15,    18,    16,     6,    27,    17,    32,
    19,    23,    30,    24,    20,    26,    28,    29
};

static const short yycheck[] = {     5,
     0,     1,     4,    14,     4,    17,    18,    17,    18,    14,
    12,    18,    12,    14,    17,    14,    16,    23,    13,     0,
    18,    18,    28,    17,    13,    18,    18,    18
};

#line 325 "/usr/lib/bison.cc"
 /* fattrs + tables */

/* parser code folow  */


/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: dollar marks section change
   the next  is replaced by the list of actions, each action
   as one case of the switch.  */ 

#if YY_FrBSyntaxicalParser_USE_GOTO != 0
/* 
 SUPRESSION OF GOTO : on some C++ compiler (sun c++)
  the goto is strictly forbidden if any constructor/destructor
  is used in the whole function (very stupid isn't it ?)
 so goto are to be replaced with a 'while/switch/case construct'
 here are the macro to keep some apparent compatibility
*/
#define YYGOTO(lb) {yy_gotostate=lb;continue;}
#define YYBEGINGOTO  enum yy_labels yy_gotostate=yygotostart; \
                     for(;;) switch(yy_gotostate) { case yygotostart: {
#define YYLABEL(lb) } case lb: {
#define YYENDGOTO } } 
#define YYBEGINDECLARELABEL enum yy_labels {yygotostart
#define YYDECLARELABEL(lb) ,lb
#define YYENDDECLARELABEL  };
#else
/* macro to keep goto */
#define YYGOTO(lb) goto lb
#define YYBEGINGOTO 
#define YYLABEL(lb) lb:
#define YYENDGOTO
#define YYBEGINDECLARELABEL 
#define YYDECLARELABEL(lb)
#define YYENDDECLARELABEL 
#endif
/* LABEL DECLARATION */
YYBEGINDECLARELABEL
  YYDECLARELABEL(yynewstate)
  YYDECLARELABEL(yybackup)
/* YYDECLARELABEL(yyresume) */
  YYDECLARELABEL(yydefault)
  YYDECLARELABEL(yyreduce)
  YYDECLARELABEL(yyerrlab)   /* here on detecting error */
  YYDECLARELABEL(yyerrlab1)   /* here on error raised explicitly by an action */
  YYDECLARELABEL(yyerrdefault)  /* current state does not do anything special for the error token. */
  YYDECLARELABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */
  YYDECLARELABEL(yyerrhandle)  
YYENDDECLARELABEL
/* ALLOCA SIMULATION */
/* __HAVE_NO_ALLOCA */
#ifdef __HAVE_NO_ALLOCA
int __alloca_free_ptr(char *ptr,char *ref)
{if(ptr!=ref) free(ptr);
 return 0;}

#define __ALLOCA_alloca(size) malloc(size)
#define __ALLOCA_free(ptr,ref) __alloca_free_ptr((char *)ptr,(char *)ref)

#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		    __ALLOCA_free(yyls,yylsa)+\
		   (num))
#else
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		   (num))
#endif
#else
#define __ALLOCA_return(num) return(num)
#define __ALLOCA_alloca(size) alloca(size)
#define __ALLOCA_free(ptr,ref) 
#endif

/* ENDALLOCA SIMULATION */

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (YY_FrBSyntaxicalParser_CHAR = YYEMPTY)
#define YYEMPTY         -2
#define YYEOF           0
#define YYACCEPT        __ALLOCA_return(0)
#define YYABORT         __ALLOCA_return(1)
#define YYERROR         YYGOTO(yyerrlab1)
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL          YYGOTO(yyerrlab)
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do                                                              \
  if (YY_FrBSyntaxicalParser_CHAR == YYEMPTY && yylen == 1)                               \
    { YY_FrBSyntaxicalParser_CHAR = (token), YY_FrBSyntaxicalParser_LVAL = (value);                 \
      yychar1 = YYTRANSLATE (YY_FrBSyntaxicalParser_CHAR);                                \
      YYPOPSTACK;                                               \
      YYGOTO(yybackup);                                            \
    }                                                           \
  else                                                          \
    { YY_FrBSyntaxicalParser_ERROR ("syntax error: cannot back up"); YYERROR; }   \
while (0)

#define YYTERROR        1
#define YYERRCODE       256

#ifndef YY_FrBSyntaxicalParser_PURE
/* UNPURE */
#define YYLEX           YY_FrBSyntaxicalParser_LEX()
#ifndef YY_USE_CLASS
/* If nonreentrant, and not class , generate the variables here */
int     YY_FrBSyntaxicalParser_CHAR;                      /*  the lookahead symbol        */
YY_FrBSyntaxicalParser_STYPE      YY_FrBSyntaxicalParser_LVAL;              /*  the semantic value of the */
				/*  lookahead symbol    */
int YY_FrBSyntaxicalParser_NERRS;                 /*  number of parse errors so far */
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
YY_FrBSyntaxicalParser_LTYPE YY_FrBSyntaxicalParser_LLOC;   /*  location data for the lookahead     */
			/*  symbol                              */
#endif
#endif


#else
/* PURE */
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
#define YYLEX           YY_FrBSyntaxicalParser_LEX(&YY_FrBSyntaxicalParser_LVAL, &YY_FrBSyntaxicalParser_LLOC)
#else
#define YYLEX           YY_FrBSyntaxicalParser_LEX(&YY_FrBSyntaxicalParser_LVAL)
#endif
#endif
#ifndef YY_USE_CLASS
#if YY_FrBSyntaxicalParser_DEBUG != 0
int YY_FrBSyntaxicalParser_DEBUG_FLAG;                    /*  nonzero means print parse trace     */
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif
#endif



/*  YYINITDEPTH indicates the initial size of the parser's stacks       */

#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif


#if __GNUC__ > 1                /* GNU C and GNU C++ define this.  */
#define __yy_bcopy(FROM,TO,COUNT)       __builtin_memcpy(TO,FROM,COUNT)
#else                           /* not GNU C or C++ */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */

#ifdef __cplusplus
static void __yy_bcopy (char *from, char *to, int count)
#else
#ifdef __STDC__
static void __yy_bcopy (char *from, char *to, int count)
#else
static void __yy_bcopy (from, to, count)
     char *from;
     char *to;
     int count;
#endif
#endif
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}
#endif

int
#ifdef YY_USE_CLASS
 YY_FrBSyntaxicalParser_CLASS::
#endif
     YY_FrBSyntaxicalParser_PARSE(YY_FrBSyntaxicalParser_PARSE_PARAM)
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
/* parameter definition without protypes */
YY_FrBSyntaxicalParser_PARSE_PARAM_DEF
#endif
#endif
#endif
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YY_FrBSyntaxicalParser_STYPE *yyvsp;
  int yyerrstatus;      /*  number of tokens to shift before error messages enabled */
  int yychar1=0;          /*  lookahead token as an internal (translated) token number */

  short yyssa[YYINITDEPTH];     /*  the state stack                     */
  YY_FrBSyntaxicalParser_STYPE yyvsa[YYINITDEPTH];        /*  the semantic value stack            */

  short *yyss = yyssa;          /*  refer to the stacks thru separate pointers */
  YY_FrBSyntaxicalParser_STYPE *yyvs = yyvsa;     /*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
  YY_FrBSyntaxicalParser_LTYPE yylsa[YYINITDEPTH];        /*  the location stack                  */
  YY_FrBSyntaxicalParser_LTYPE *yyls = yylsa;
  YY_FrBSyntaxicalParser_LTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YY_FrBSyntaxicalParser_PURE
  int YY_FrBSyntaxicalParser_CHAR;
  YY_FrBSyntaxicalParser_STYPE YY_FrBSyntaxicalParser_LVAL;
  int YY_FrBSyntaxicalParser_NERRS;
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
  YY_FrBSyntaxicalParser_LTYPE YY_FrBSyntaxicalParser_LLOC;
#endif
#endif

  YY_FrBSyntaxicalParser_STYPE yyval;             /*  the variable used to return         */
				/*  semantic values from the action     */
				/*  routines                            */

  int yylen;
/* start loop, in which YYGOTO may be used. */
YYBEGINGOTO

#if YY_FrBSyntaxicalParser_DEBUG != 0
  if (YY_FrBSyntaxicalParser_DEBUG_FLAG)
    fprintf(stderr, "Starting parse\n");
#endif
  yystate = 0;
  yyerrstatus = 0;
  YY_FrBSyntaxicalParser_NERRS = 0;
  YY_FrBSyntaxicalParser_CHAR = YYEMPTY;          /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
YYLABEL(yynewstate)

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YY_FrBSyntaxicalParser_STYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
      YY_FrBSyntaxicalParser_LTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  YY_FrBSyntaxicalParser_ERROR("parser stack overflow");
	  __ALLOCA_return(2);
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) __ALLOCA_alloca (yystacksize * sizeof (*yyssp));
      __yy_bcopy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      __ALLOCA_free(yyss1,yyssa);
      yyvs = (YY_FrBSyntaxicalParser_STYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yyvsp));
      __yy_bcopy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
      __ALLOCA_free(yyvs1,yyvsa);
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
      yyls = (YY_FrBSyntaxicalParser_LTYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yylsp));
      __yy_bcopy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
      __ALLOCA_free(yyls1,yylsa);
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YY_FrBSyntaxicalParser_DEBUG != 0
      if (YY_FrBSyntaxicalParser_DEBUG_FLAG)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YY_FrBSyntaxicalParser_DEBUG != 0
  if (YY_FrBSyntaxicalParser_DEBUG_FLAG)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  YYGOTO(yybackup);
YYLABEL(yybackup)

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* YYLABEL(yyresume) */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yydefault);

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (YY_FrBSyntaxicalParser_CHAR == YYEMPTY)
    {
#if YY_FrBSyntaxicalParser_DEBUG != 0
      if (YY_FrBSyntaxicalParser_DEBUG_FLAG)
	fprintf(stderr, "Reading a token: ");
#endif
      YY_FrBSyntaxicalParser_CHAR = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (YY_FrBSyntaxicalParser_CHAR <= 0)           /* This means end of input. */
    {
      yychar1 = 0;
      YY_FrBSyntaxicalParser_CHAR = YYEOF;                /* Don't call YYLEX any more */

#if YY_FrBSyntaxicalParser_DEBUG != 0
      if (YY_FrBSyntaxicalParser_DEBUG_FLAG)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(YY_FrBSyntaxicalParser_CHAR);

#if YY_FrBSyntaxicalParser_DEBUG != 0
      if (YY_FrBSyntaxicalParser_DEBUG_FLAG)
	{
	  fprintf (stderr, "Next token is %d (%s", YY_FrBSyntaxicalParser_CHAR, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, YY_FrBSyntaxicalParser_CHAR, YY_FrBSyntaxicalParser_LVAL);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    YYGOTO(yydefault);

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrlab);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrlab);

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YY_FrBSyntaxicalParser_DEBUG != 0
  if (YY_FrBSyntaxicalParser_DEBUG_FLAG)
    fprintf(stderr, "Shifting token %d (%s), ", YY_FrBSyntaxicalParser_CHAR, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (YY_FrBSyntaxicalParser_CHAR != YYEOF)
    YY_FrBSyntaxicalParser_CHAR = YYEMPTY;

  *++yyvsp = YY_FrBSyntaxicalParser_LVAL;
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
  *++yylsp = YY_FrBSyntaxicalParser_LLOC;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  YYGOTO(yynewstate);

/* Do the default action for the current state.  */
YYLABEL(yydefault)

  yyn = yydefact[yystate];
  if (yyn == 0)
    YYGOTO(yyerrlab);

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
YYLABEL(yyreduce)
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YY_FrBSyntaxicalParser_DEBUG != 0
  if (YY_FrBSyntaxicalParser_DEBUG_FLAG)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


/* #line 811 "/usr/lib/bison.cc" */
#line 1078 "syntaxer.tab.c"

  switch (yyn) {

case 5:
#line 50 "syntaxer.y"
{ yyerrok; ;
    break;}
case 6:
#line 53 "syntaxer.y"
{  ;
    break;}
case 7:
#line 54 "syntaxer.y"
{  ;
    break;}
case 8:
#line 56 "syntaxer.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 9:
#line 57 "syntaxer.y"
{ yyval = yyvsp[0]; ;
    break;}
case 10:
#line 60 "syntaxer.y"
{ /* prise en charge complete du Dim ... */ ;
    break;}
case 13:
#line 65 "syntaxer.y"
{ /* id, id, ... As id */ ;
    break;}
case 14:
#line 66 "syntaxer.y"
{ /* id As id = expr*/ ;
    break;}
case 15:
#line 69 "syntaxer.y"
{ /* id, id, ... */ ;
    break;}
}

#line 811 "/usr/lib/bison.cc"
   /* the action file gets copied in in place of this dollarsign  */
  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
  yylsp -= yylen;
#endif

#if YY_FrBSyntaxicalParser_DEBUG != 0
  if (YY_FrBSyntaxicalParser_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = YY_FrBSyntaxicalParser_LLOC.first_line;
      yylsp->first_column = YY_FrBSyntaxicalParser_LLOC.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  YYGOTO(yynewstate);

YYLABEL(yyerrlab)   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++YY_FrBSyntaxicalParser_NERRS;

#ifdef YY_FrBSyntaxicalParser_ERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      YY_FrBSyntaxicalParser_ERROR(msg);
	      free(msg);
	    }
	  else
	    YY_FrBSyntaxicalParser_ERROR ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YY_FrBSyntaxicalParser_ERROR_VERBOSE */
	YY_FrBSyntaxicalParser_ERROR("parse error");
    }

  YYGOTO(yyerrlab1);
YYLABEL(yyerrlab1)   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (YY_FrBSyntaxicalParser_CHAR == YYEOF)
	YYABORT;

#if YY_FrBSyntaxicalParser_DEBUG != 0
      if (YY_FrBSyntaxicalParser_DEBUG_FLAG)
	fprintf(stderr, "Discarding token %d (%s).\n", YY_FrBSyntaxicalParser_CHAR, yytname[yychar1]);
#endif

      YY_FrBSyntaxicalParser_CHAR = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;              /* Each real token shifted decrements this */

  YYGOTO(yyerrhandle);

YYLABEL(yyerrdefault)  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) YYGOTO(yydefault);
#endif

YYLABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
  yylsp--;
#endif

#if YY_FrBSyntaxicalParser_DEBUG != 0
  if (YY_FrBSyntaxicalParser_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

YYLABEL(yyerrhandle)

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yyerrdefault);

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    YYGOTO(yyerrdefault);

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrpop);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrpop);

  if (yyn == YYFINAL)
    YYACCEPT;

#if YY_FrBSyntaxicalParser_DEBUG != 0
  if (YY_FrBSyntaxicalParser_DEBUG_FLAG)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = YY_FrBSyntaxicalParser_LVAL;
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
  *++yylsp = YY_FrBSyntaxicalParser_LLOC;
#endif

  yystate = yyn;
  YYGOTO(yynewstate);
/* end loop, in which YYGOTO may be used. */
  YYENDGOTO
}

/* END */

/* #line 1010 "/usr/lib/bison.cc" */
#line 1322 "syntaxer.tab.c"
#line 78 "syntaxer.y"
