#ifndef YY_FrBSyntaxicalParser_h_included
#define YY_FrBSyntaxicalParser_h_included

#line 1 "/usr/lib/bison.h"
/* before anything */
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
#endif
#include <stdio.h>

/* #line 14 "/usr/lib/bison.h" */
#line 21 "syntaxer.tab.h"

#line 14 "/usr/lib/bison.h"
 /* %{ and %header{ and %union, during decl */
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
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
/* use %define LTYPE */
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_FrBSyntaxicalParser_STYPE 
#define YY_FrBSyntaxicalParser_STYPE YYSTYPE
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
/* use %define STYPE */
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_FrBSyntaxicalParser_DEBUG
#define  YY_FrBSyntaxicalParser_DEBUG YYDEBUG
/* WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
/* use %define DEBUG */
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

/* #line 63 "/usr/lib/bison.h" */
#line 75 "syntaxer.tab.h"

#line 63 "/usr/lib/bison.h"
/* YY_FrBSyntaxicalParser_PURE */
#endif

/* #line 65 "/usr/lib/bison.h" */
#line 82 "syntaxer.tab.h"

#line 65 "/usr/lib/bison.h"
/* prefix */
#ifndef YY_FrBSyntaxicalParser_DEBUG

/* #line 67 "/usr/lib/bison.h" */
#line 89 "syntaxer.tab.h"

#line 67 "/usr/lib/bison.h"
/* YY_FrBSyntaxicalParser_DEBUG */
#endif
#ifndef YY_FrBSyntaxicalParser_LSP_NEEDED

/* #line 70 "/usr/lib/bison.h" */
#line 97 "syntaxer.tab.h"

#line 70 "/usr/lib/bison.h"
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

/* TOKEN C */
#ifndef YY_USE_CLASS

#ifndef YY_FrBSyntaxicalParser_PURE
extern YY_FrBSyntaxicalParser_STYPE YY_FrBSyntaxicalParser_LVAL;
#endif


/* #line 143 "/usr/lib/bison.h" */
#line 175 "syntaxer.tab.h"
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


#line 143 "/usr/lib/bison.h"
 /* #defines token */
/* after #define tokens, before const tokens S5*/
#else
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

/* #line 182 "/usr/lib/bison.h" */
#line 237 "syntaxer.tab.h"
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


#line 182 "/usr/lib/bison.h"
 /* decl const */
#else
enum YY_FrBSyntaxicalParser_ENUM_TOKEN { YY_FrBSyntaxicalParser_NULL_TOKEN=0

/* #line 185 "/usr/lib/bison.h" */
#line 263 "syntaxer.tab.h"
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


#line 185 "/usr/lib/bison.h"
 /* enum token */
     }; /* end of enum declaration */
#endif
public:
 int YY_FrBSyntaxicalParser_PARSE(YY_FrBSyntaxicalParser_PARSE_PARAM);
 virtual void YY_FrBSyntaxicalParser_ERROR(char *msg) YY_FrBSyntaxicalParser_ERROR_BODY;
#ifdef YY_FrBSyntaxicalParser_PURE
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
 virtual int  YY_FrBSyntaxicalParser_LEX(YY_FrBSyntaxicalParser_STYPE *YY_FrBSyntaxicalParser_LVAL,YY_FrBSyntaxicalParser_LTYPE *YY_FrBSyntaxicalParser_LLOC) YY_FrBSyntaxicalParser_LEX_BODY;
#else
 virtual int  YY_FrBSyntaxicalParser_LEX(YY_FrBSyntaxicalParser_STYPE *YY_FrBSyntaxicalParser_LVAL) YY_FrBSyntaxicalParser_LEX_BODY;
#endif
#else
 virtual int YY_FrBSyntaxicalParser_LEX() YY_FrBSyntaxicalParser_LEX_BODY;
 YY_FrBSyntaxicalParser_STYPE YY_FrBSyntaxicalParser_LVAL;
#ifdef YY_FrBSyntaxicalParser_LSP_NEEDED
 YY_FrBSyntaxicalParser_LTYPE YY_FrBSyntaxicalParser_LLOC;
#endif
 int YY_FrBSyntaxicalParser_NERRS;
 int YY_FrBSyntaxicalParser_CHAR;
#endif
#if YY_FrBSyntaxicalParser_DEBUG != 0
public:
 int YY_FrBSyntaxicalParser_DEBUG_FLAG;	/*  nonzero means print parse trace	*/
#endif
public:
 YY_FrBSyntaxicalParser_CLASS(YY_FrBSyntaxicalParser_CONSTRUCTOR_PARAM);
public:
 YY_FrBSyntaxicalParser_MEMBERS 
};
/* other declare folow */
#endif


#if YY_FrBSyntaxicalParser_COMPATIBILITY != 0
/* backward compatibility */
#ifndef YYSTYPE
#define YYSTYPE YY_FrBSyntaxicalParser_STYPE
#endif

#ifndef YYLTYPE
#define YYLTYPE YY_FrBSyntaxicalParser_LTYPE
#endif
#ifndef YYDEBUG
#ifdef YY_FrBSyntaxicalParser_DEBUG 
#define YYDEBUG YY_FrBSyntaxicalParser_DEBUG
#endif
#endif

#endif
/* END */

/* #line 236 "/usr/lib/bison.h" */
#line 337 "syntaxer.tab.h"
#endif
