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

#undef yyFlexLexer //lexical parser
#define yyFlexLexer FrBFlexLexer
#include <FlexLexer.h>

#include "frblexer.h"
#include "frbsyntaxer.h"

#include <cstdio>

/*
    Les mots clefs déclarent une unique fois dans frbkeywords.key, les
    constantes suivantes sont automatiquement définies lors du make :
    
    FRB_KW_TOKEN_* correspond au token des mot clefs, déclarés ci dessous
                   par DECLARE_KEYWORDS_TOKEN (utilisés par le syntaxer)
                   
    FRB_KW_*       correspond au mot clefs declarés dans FrBKeywords
                   (utilisés dans le lexer
*/
    
/* Inherited attr : p189 */


/* il faut freer tous les IDENTIFICATER et seulement les identificateurs */

%}

%header{

#include "frbexpr.h"
#include "frbstat.h"
#include "frberrors.h"
#include "frberrordesc.h"
#include "frbkeywords.h"
#include "frbfunction.h"
#include "frbclass.h"
#include "../common/assert.h"

struct FnCParam
{
    FrBTypeExpr * type;
    CString       str;
};

struct FnAttr
{
    int   scope;
    int   class_attr;
    bool  is_const;
};

enum { FN_UNKNOW, FN_NORMAL, FN_CTOR, FN_DTOR };


%}

%name FrBSyntaxicalParser

%union
{
    CString                    str;
    FrBBaseObject *            obj;
    FrBExpr *                  expr;
    FrBUnresolvedTypeExpr *    vtype;
    FrBStatement *             stat;
    FrBExprList*               exprs;
    int                        vint;
    FnCParam                   param;
    bool                       vbool;
    FnAttr                     fnattr;
}

%start program_file

%token FRB_TYPE_LITERAL_DOUBLE
%token FRB_TYPE_LITERAL_SINGLE
%token FRB_TYPE_LITERAL_INT
%token FRB_TYPE_LITERAL_LONGINT
%token FRB_TYPE_LITERAL_SHORTINT
%token FRB_TYPE_LITERAL_STRING
%token FRB_TYPE_LITERAL_CHAR

%token FRB_END_LINE
%token FRB_END_OF_FILE

%token FRB_IDENTIFIER

/* mots clefs déclarés dans frbkeywords.key sauf les opérateurs */
%DECLARE_KEYWORDS_TOKEN

/* Priorité des opérateurs */

/* 1. Opérateur de concaténation */
%left FRB_KW_TOKEN_OP_CONCAT

/* 4. Opérateurs d'assignement */
%right FRB_KW_TOKEN_OP_ASSIGN_VAL FRB_KW_TOKEN_OP_ASSIGN_REF FRB_KW_TOKEN_OP_ADD_ASSIGN FRB_KW_TOKEN_OP_SUB_ASSIGN FRB_KW_TOKEN_OP_MUL_ASSIGN FRB_KW_TOKEN_OP_DIV_ASSIGN FRB_KW_TOKEN_OP_INT_DIV_ASSIGN FRB_KW_TOKEN_OP_POW_ASSIGN

/* 3. Opérateurs logiques */
%left FRB_KW_TOKEN_OP_LOG_AND FRB_KW_TOKEN_OP_LOG_OR FRB_KW_TOKEN_OP_LOG_XOR FRB_KW_TOKEN_OP_LOG_NOT

/* 2. Opérateurs de comparaison */
%left FRB_KW_TOKEN_OP_EQ FRB_KW_TOKEN_OP_NE FRB_KW_TOKEN_OP_LT FRB_KW_TOKEN_OP_GT FRB_KW_TOKEN_OP_LE FRB_KW_TOKEN_OP_GE


/* 5. Opérateurs arithmétiques */
%left FRB_KW_TOKEN_OP_ADD FRB_KW_TOKEN_OP_SUB
%left FRB_KW_TOKEN_OP_MOD
%left FRB_KW_TOKEN_OP_DIV FRB_KW_TOKEN_OP_MUL FRB_KW_TOKEN_OP_INT_DIV
%right FRB_KW_TOKEN_OP_POW

/* 6. Opérateur membre */
%left FRB_KW_TOKEN_OP_MEMBER

/* 7. Opérateur d'objet */
%left FRB_KW_TOKEN_OP_IS FRB_KW_TOKEN_OP_IN FRB_KW_TOKEN_OP_TYPEOF FRB_KW_TOKEN_OP_INSTANCEOF FRB_KW_TOKEN_OP_INHERITS
%nonassoc FRB_KW_TOKEN_OP_SIZEOF

/* 8. Operateurs unaires */
%nonassoc FRB_KW_TOKEN_OP_UNARY_MINUS FRB_KW_TOKEN_OP_CARD  FRB_KW_TOKEN_INCR FRB_KW_TOKEN_OP_DECR


/* 9. Parenthèses */
%nonassoc FRB_KW_TOKEN_OP_O_BRACKET FRB_KW_TOKEN_OP_ARRAY_SUB_BEGIN
%%



new_line:
       FRB_KW_TOKEN_OP_NEWLINE /* opérateur nouvelle ligne, ':' par défaut */
     | FRB_END_LINE /* nouvelle ligne, \r */
     ;

program_file:
       main_stat_list FRB_END_OF_FILE { YYACCEPT; }
     | FRB_END_OF_FILE                { YYACCEPT; }
     ;
    
main_stat_list:
       main_stat_list class
     | main_stat_list limited_typedef_stat
     | main_stat_list new_line
     | class
     | limited_typedef_stat
     | new_line
     ;

limited_typedef_stat:
       FRB_KW_TOKEN_TYPEDEF FRB_IDENTIFIER FRB_IDENTIFIER new_line /* TypeDef type Id */
     | FRB_KW_TOKEN_TYPEUNDEF FRB_IDENTIFIER new_line /* TypeUnDef Id */
     ;
  
typedef_stat:
       FRB_KW_TOKEN_TYPEDEF type FRB_IDENTIFIER new_line /* TypeDef type Id */
     | FRB_KW_TOKEN_TYPEUNDEF FRB_IDENTIFIER new_line /* TypeUnDef Id */
     ;
     
class_attr:
      FRB_KW_TOKEN_SHARED   { $<vint>$ = SC_SHARED; }
    | FRB_KW_TOKEN_ABSTRACT { $<vint>$ = SC_ABSTRACT; }
    | /* empty */           { $<vint>$ = SC_DEFAULT; }
    ;
    
class_sealed:
      FRB_KW_TOKEN_SEALED { $<vint>$ = SC_SEALED; }
    | /* empty */         { $<vint>$ = SC_DEFAULT; }
    ;
 
class_type:
      FRB_KW_TOKEN_CLASS      { $<vint>$ = FRB_KW_TOKEN_CLASS; }
    | FRB_KW_TOKEN_INTERFACE  { $<vint>$ = FRB_KW_TOKEN_INTERFACE; }
    | FRB_KW_TOKEN_MODULE     { $<vint>$ = FRB_KW_TOKEN_MODULE; }

class:
       member_scope_attr /*1*/ class_attr /*2*/ class_sealed /*3*/ class_type /*4*/ FRB_IDENTIFIER /*5*/ /* Class id */
       {
           FrBCodeClass * nclass = new FrBCodeClass();
           
           nclass->setName(String($<str>5));
           free($<str>5);
           nclass->setScope($<vint>1);
           
           //TODO reverifier tout ca et mettre des if plutot
           nclass->setShared( ($<vint>4 == FRB_KW_TOKEN_MODULE) || ($<vint>2 == SC_SHARED) );
           nclass->setAbstract( ($<vint>4 == FRB_KW_TOKEN_INTERFACE) || ($<vint>2 == SC_ABSTRACT) );
           nclass->setSealed( $<vint>3 == SC_SEALED );
           
           if(class_stack.empty())
               (*frb_classes)[nclass->name()] = nclass;
           else
               class_stack.top()->addInnerClass(nclass);
           
           class_stack.push(nclass);
       } /*6*/
       class_content_list /*7*/
       FRB_KW_TOKEN_END /*8*/ class_type /*9*/ new_line /*10*/ /* End Class */
       {
           if( $<vint>4 != $<vint>9 )
           {
               frb_error->error(FrBErrors::FRB_ERR_CLASS_DEC_END,
                                    FrBErrors::FRB_ERR_PARSE,
                                    frb_lexer->lineno(), "", "", "",
                                    String(frb_lexer->YYText()));

               YYABORT;
           }
           
           class_stack.pop();
       }  
       
     ;

class_content_list:
       class_content_list member
     | class_content_list class
     | class_content_list typedef_stat
     | class_content_list new_line
     | member
     | class
     | typedef_stat
     | new_line
     ;


member:
      function
    | data_member /* event aussi */
    ;


member_scope_attr:
      FRB_KW_TOKEN_PRIVATE     { $<vint>$ = SC_DEFAULT; }
    | FRB_KW_TOKEN_PUBLIC      { $<vint>$ = SC_PUBLIC ; }
    | FRB_KW_TOKEN_PROTECTED   { $<vint>$ = SC_PROTECTED; }
    | FRB_KW_TOKEN_FRIEND      { $<vint>$ = SC_FRIEND; }
    //| /* empty  */             { $<vint>$ = SC_DEFAULT; }
    ;

fn_class_attr:
      FRB_KW_TOKEN_SHARED       { $<vint>$ = SC_SHARED; }
    | FRB_KW_TOKEN_ABSTRACT     { $<vint>$ = SC_ABSTRACT; }
    | /* empty */               { $<vint>$ = SC_DEFAULT; }
    ;    

fn_const_attr:
      FRB_KW_TOKEN_CONST  { $<vbool>$ = true; }
    | /* empty  */        { $<vbool>$ = false; }
    ;
    
fn_attr:
      member_scope_attr fn_class_attr fn_const_attr
      {
          $<fnattr>$.scope = $<vint>1;
          $<fnattr>$.class_attr = $<vint>2;
          $<fnattr>$.is_const = $<vbool>3;
      }
    ;
    
      /*Public|Private|Protected|Friend [Shared|Virtual|PureVirtual] [Const] [Function|Sub|Operator|Property (Set|Get)] id( [ByVal|ByRef] id As type,
         [Optional id As type = expr]|[ParamArray args[] As type) As type*/ 

/*function_type:
      FRB_KW_TOKEN_FUNCTION
    | FRB_KW_TOKEN_PROPERTY FRB_KW_TOKEN_GET
    ;
   
sub_type:
      FRB_KW_TOKEN_SUB
    | FRB_KW_TOKEN_PROPERTY FRB_KW_TOKEN_SET
    ;*/
    
sub_name:
       FRB_IDENTIFIER { $<str>$ = $<str>1; }
     | FRB_KW_TOKEN_CONSTRUCTOR_NAME  { $<str>$ = $<str>1; }
     | FRB_KW_TOKEN_DESTRUCTOR_NAME  { $<str>$ = $<str>1; }


function_type:
       FRB_KW_TOKEN_SUB { $<str>$ = $<str>1; }
     | FRB_KW_TOKEN_FUNCTION { $<str>$ = $<str>1; }

 
function:
      fn_attr /*1*/ function_type /*2*/ sub_name /*3*/
      { /*4*/

	   $<vint>$ = FN_UNKNOW;

           frb_assert2(!class_stack.empty(), "fn/sub declaration");
           
           FrBCodeFunction * nfn = new FrBCodeFunction();

           nfn->setContainer(current_class());

           nfn->setSub($<str>2 == FrBKeywords::getKeyword(FrBKeywords::FRB_KW_SUB));
           nfn->setName(String($<str>3));
           
           nfn->setScope( (scope_t)($<fnattr>1.scope) );
           nfn->setShared( $<fnattr>1.class_attr == SC_SHARED );
           nfn->setAbstract( $<fnattr>1.class_attr == SC_ABSTRACT );
           nfn->setConst( $<fnattr>1.is_const );
           
           
           //$<fnattr>$
               
                              
           
           if(fn_stack.empty())
           {

               if( $<str>3 == FrBKeywords::getKeyword(FrBKeywords::FRB_KW_CONSTRUCTOR_NAME))
               {
                   //TODO: verifier les parametres/retour
                   current_class()->addConstructor(nfn, true);
	   	   $<vint>$ = FN_CTOR;
               }
               else if( $<str>3 == FrBKeywords::getKeyword(FrBKeywords::FRB_KW_DESTRUCTOR_NAME))
               {
                   current_class()->addDestructor(nfn);
		   $<vint>$ = FN_DTOR;
               }
               else
               {
                   current_class()->addFunction(nfn);
                   free($<str>3);
		   $<vint>$ = FN_NORMAL;
               }
                   
               
           }
           else
           {
               frb_assert2(false, "inner functions not implemented");
               free($<str>3);
           }
               
           fn_stack.push(nfn);
	   block_stack.push(nfn);
           
      }
      fn_arg_list /*5*/ as_type_optionnal /*6*/ /* sub/function */
      { /*7*/
	   FrBCodeFunction * f = current_fn();

	   if($<vint>4 == FN_CTOR && f->parameterCount() == 0)
	       current_class()->setDefaultCtor(f);

           if(f->sub() && $<vtype>6 != 0)
                frb_error->error(FrBErrors::FRB_ERR_SUB_RETURN_TYPE,
                    FrBErrors::FRB_ERR_PARSE,
                    frb_lexer->lineno(), "", "", "",
                    String(frb_lexer->YYText()));
            else if(!f->sub() && $<vtype>6 == 0)
                frb_error->error(FrBErrors::FRB_ERR_FN_RETURN_TYPE,
                    FrBErrors::FRB_ERR_PARSE,
                    frb_lexer->lineno(), "", "", "",
                    String(frb_lexer->YYText()));
                    
            f->setURReturnType($<vtype>6);        
           
      }
      function_content_list /*8*/
      FRB_KW_TOKEN_END /*9*/ function_type /*10*/
      { /*11*/   
          if($<str>2 != $<str>10)
                frb_error->error(FrBErrors::FRB_ERR_FN_DEC_END,
                    FrBErrors::FRB_ERR_PARSE,
                    frb_lexer->lineno(), "", "", "",
                    String(frb_lexer->YYText()));
           
	  /* does all path contain a return statement ? */
	  if( !current_fn()->sub() && !current_fn()->allPathContainsAReturn())
            frb_error->error(FrBErrors::FRB_ERR_NO_RETURN_FUNCTION,
			     FrBErrors::FRB_ERR_SEMANTIC,
			     frb_lexer->lineno(), "", "", "",
			     String(frb_lexer->YYText()) );

          fn_stack.pop();
	  block_stack.pop();
      }
      
    | fn_attr FRB_KW_TOKEN_OPERATOR operator_overloadable fn_arg_list as_type /* operator (function) */
      function_content_list
      FRB_KW_TOKEN_END FRB_KW_TOKEN_OPERATOR
      
    | fn_attr FRB_KW_TOKEN_PROPERTY FRB_KW_TOKEN_GET FRB_IDENTIFIER fn_arg_list as_type /* autoconst property get  (function) */
      function_content_list
      FRB_KW_TOKEN_END FRB_KW_TOKEN_PROPERTY
       
    | fn_attr FRB_KW_TOKEN_PROPERTY FRB_KW_TOKEN_SET FRB_IDENTIFIER fn_arg_list /* property set */
      function_content_list
      FRB_KW_TOKEN_END FRB_KW_TOKEN_PROPERTY     
    ;
    
function_content_list:
       function_content_list fn_stat
     | function_content_list new_line
     | fn_stat
     | new_line
     ;
     
fn_stat: /* stat new_line */
      expr new_line /* expression */
      {
	//TODO warning if $<expr>1->getClass() != 0 'result lost
	current_block()->addStat(new FrBExprStatement($<expr>1));
      }
    | dim_stat new_line /* déclaration */
    | if_stat /* if */
    | delete_stat new_line /* destruction */
    | return_stat new_line /* return  */
    | typedef_stat /* typedef */
    | for_loop new_line /* for */
    ;
    
if_def: /* if <expr> then <lf> stats */
      FRB_KW_TOKEN_IF expr FRB_KW_TOKEN_THEN new_line
      {
	current_if = new FrBIfStatement();
	current_block()->addStat(current_if);

	FrBElseIfStatement * b = new FrBElseIfStatement($<expr>2);

	current_if->addCond(b);
	block_stack.push(b);
      }
      function_content_list
      { block_stack.pop(); }
    ;

elseif_def: /* else if <expr> then <lf> stats */
      FRB_KW_TOKEN_ELSE FRB_KW_TOKEN_IF expr FRB_KW_TOKEN_THEN new_line
      {
	FrBElseIfStatement * b = new FrBElseIfStatement($<expr>3);

	current_if->addCond(b);
	block_stack.push(b);
      }
      function_content_list
      { block_stack.pop(); }
    ;

else_def: /* else <lf> stats or nothing */
      FRB_KW_TOKEN_ELSE new_line
      {
	FrBElseStatement * b = new FrBElseStatement();

	current_if->addCond(b);
	current_if->setHasElse(true);
	block_stack.push(b);
      }
      function_content_list
      { block_stack.pop(); }
    | /* empty */
    ;

elseif_list:
      elseif_list elseif_def
    | /* empty */
    ;

if_stat:
      if_def
      elseif_list
      else_def
      FRB_KW_TOKEN_END FRB_KW_TOKEN_IF new_line
    | FRB_KW_TOKEN_IF expr FRB_KW_TOKEN_THEN /* inline if */
      {
	FrBElseIfStatement * b = new FrBElseIfStatement($<expr>2);

	current_block()->addStat(b);
	block_stack.push(b);
      }
      fn_stat { block_stack.pop(); }
      /*    | FRB_KW_TOKEN_IF expr FRB_KW_TOKEN_THEN fn_stat FRB_KW_TOKEN_ELSE fn_stat */
    ;
    
    
dim_stat: /* Dim nom1_1, nom1_2, ... As type [= init], nom2_1, nom2_2, ... As type [= init], ... */
      FRB_KW_TOKEN_DECLARE declare_list
    ;

declare_init_nonempty:
      FRB_KW_TOKEN_OP_ASSIGN_VAL expr
      {
            frb_error->error(FrBErrors::FRB_ERR_DECLARE_INIT_ASSIGN,
            FrBErrors::FRB_ERR_PARSE,
            frb_lexer->lineno(), "", "", "",
            String(frb_lexer->YYText()));
            
            delete $<expr>2;
            $<expr>$ = 0;
      }
    | FRB_KW_TOKEN_OP_ASSIGN_REF expr { $<expr>$ = $<expr>2; }
    ;

declare_init:
      declare_init_nonempty           { $<expr>$ = $<expr>1; }
    | /* empty */                     { $<expr>$ = 0; }
    ;

declare_list: /* nom1_1, nom1_2, ... As type [= init], nom2_1, nom2_2, ... As type [= init], ... */
      declare_list FRB_KW_TOKEN_OP_LIST_SEP identifier_list as_type declare_init
      {
            /* /!\ doublon */
            FrBCodeFunction * fn = current_fn();

            FrBDeclareStatement * d = new FrBDeclareStatement(fn, id_list.size(),
                                                                $<vtype>4, $<expr>5);
	    current_block()->addStat(d);

            for(CStringList::iterator it = id_list.begin(); it != id_list.end(); ++it)
            {
                int var_count = fn->localVarCount();

                d->addVarID(-var_count - 1);

                fn->addLocalVar((*it), $<vtype>4);

                if(fn->localVarCount() == var_count || fn->getParam(*it) != -1)
                    frb_error->error(FrBErrors::FRB_ERR_REDECL_ID,
                    FrBErrors::FRB_ERR_SEMANTIC,
                    frb_lexer->lineno(), "", "", "",
                    String(*it));

                free(*it);
            }
            id_list.clear();
      }
    | identifier_list as_type declare_init
      {
            /* /!\ doublon */
            FrBCodeFunction * fn = current_fn();

            FrBDeclareStatement * d = new FrBDeclareStatement(fn, id_list.size(),
                                                                $<vtype>2, $<expr>3);
	    current_block()->addStat(d);

            for(CStringList::iterator it = id_list.begin(); it != id_list.end(); ++it)
            {
                int var_count = fn->localVarCount();

                d->addVarID(-var_count - 1);

                fn->addLocalVar((*it), $<vtype>2);

                if(fn->localVarCount() == var_count || fn->getParam(*it) != -1)
                    frb_error->error(FrBErrors::FRB_ERR_REDECL_ID,
                    FrBErrors::FRB_ERR_SEMANTIC,
                    frb_lexer->lineno(), "", "", "",
                    String(*it));

                free(*it);
            }
            id_list.clear();
      }
    ;

identifier_list: /* nom1, nom2, ... */
      identifier_list FRB_KW_TOKEN_OP_LIST_SEP FRB_IDENTIFIER
      {
          id_list.push_back($<str>3);
          //free($<str>3);
      }
      
    | FRB_IDENTIFIER
      {
          id_list.push_back($<str>1);
          //free($<str>1);
      }
    ;
    
    
delete_stat: /* Delete expr */
      FRB_KW_TOKEN_OP_DELETE expr
    ;

return_stat: /* Return expr */
      FRB_KW_TOKEN_RETURN expr
     {
       if(current_fn()->sub())
	  frb_error->error(FrBErrors::FRB_ERR_BAD_RETURN_SUB,
			   FrBErrors::FRB_ERR_SEMANTIC,
			   frb_lexer->lineno(), "", "", "",
			   String(frb_lexer->YYText()));

       current_block()->addStat(new FrBReturnStatement(current_fn(), $<expr>2));
     }
    | FRB_KW_TOKEN_RETURN
      {
       if(!current_fn()->sub())
	  frb_error->error(FrBErrors::FRB_ERR_BAD_RETURN_FUNCTION,
			   FrBErrors::FRB_ERR_SEMANTIC,
			   frb_lexer->lineno(), "", "", "",
			   String(frb_lexer->YYText()));

       current_block()->addStat(new FrBReturnStatement(current_fn()));
     }

    ;
    

    
operator_overloadable:
      FRB_KW_TOKEN_OP_ASSIGN_VAL
    | FRB_KW_TOKEN_OP_ADD_ASSIGN
    | FRB_KW_TOKEN_OP_SUB_ASSIGN
    | FRB_KW_TOKEN_OP_MUL_ASSIGN
    | FRB_KW_TOKEN_OP_DIV_ASSIGN
    | FRB_KW_TOKEN_OP_INCR
    | FRB_KW_TOKEN_OP_DECR
    | FRB_KW_TOKEN_OP_SUB
    | FRB_KW_TOKEN_OP_ADD
    | FRB_KW_TOKEN_OP_MUL
    | FRB_KW_TOKEN_OP_DIV
    | FRB_KW_TOKEN_OP_INT_DIV
    | FRB_KW_TOKEN_OP_POW
    | FRB_KW_TOKEN_OP_EQ
    | FRB_KW_TOKEN_OP_NE
    | FRB_KW_TOKEN_OP_LT
    | FRB_KW_TOKEN_OP_GT
    | FRB_KW_TOKEN_OP_LE
    | FRB_KW_TOKEN_OP_GE
    | FRB_KW_TOKEN_OP_O_BRACKET FRB_KW_TOKEN_OP_C_BRACKET
    | FRB_KW_TOKEN_OP_ARRAY_SUB_BEGIN FRB_KW_TOKEN_OP_ARRAY_SUB_END
    | FRB_KW_TOKEN_OP_BITW_LSHIFT
    | FRB_KW_TOKEN_OP_BITW_RSHIFT
    | FRB_KW_TOKEN_OP_MOD
    | FRB_KW_TOKEN_OP_LOG_AND
    | FRB_KW_TOKEN_OP_LOG_OR
    | FRB_KW_TOKEN_OP_LOG_XOR
    | FRB_KW_TOKEN_OP_LOG_NOT
    ;

fn_arg_list: /* ( [arg list] )  */
      FRB_KW_TOKEN_OP_O_BRACKET arg_list FRB_KW_TOKEN_OP_C_BRACKET
    | FRB_KW_TOKEN_OP_O_BRACKET /* empty */ FRB_KW_TOKEN_OP_C_BRACKET
    ;

arg_mode:
      FRB_KW_TOKEN_BYREF { $<vbool>$ = false; }
    | FRB_KW_TOKEN_BYVAL { $<vbool>$ = true; }
    | /* empty */        { $<vbool>$ = true; }
    ;
    
arg_opt:
      FRB_KW_TOKEN_OP_ASSIGN_REF expr { $<expr>$ = $<expr>2; }
    | /* empty */                     { $<expr>$ = 0; }
    ;
    
arg_type:
      FRB_KW_TOKEN_OPTIONAL arg_mode arg arg_opt /* Optional [ByRef|ByVal] id As Type [:= init] */
      {
          if(current_fn()->paramArrayUsed())
                frb_error->error(FrBErrors::FRB_ERR_PARAMARRAY_LAST,
                        FrBErrors::FRB_ERR_PARSE,
                        frb_lexer->lineno(), "", "", "",
                        String(frb_lexer->YYText()));
      
          if(current_fn()->firstOptionalParameter() == -1)
              current_fn()->setFirstOptionalParameter(current_fn()->parameterCount());
             
          
          current_fn()->addParam(String($<param>3.str), $<param>3.type, $<vbool>2, $<expr>4);
          free($<param>3.str);
      }
    | FRB_KW_TOKEN_PARAMARRAY arg_mode arg /* ParamArray [ByRef|ByVal] id As Type */
      {
          if(current_fn()->firstOptionalParameter() > -1)
                frb_error->error(FrBErrors::FRB_ERR_PARAMARRAY_OPT,
                        FrBErrors::FRB_ERR_PARSE,
                        frb_lexer->lineno(), "", "", "",
                        String(frb_lexer->YYText()));
             
          if(current_fn()->paramArrayUsed())
                frb_error->error(FrBErrors::FRB_ERR_PARAMARRAY_LAST,
                        FrBErrors::FRB_ERR_PARSE,
                        frb_lexer->lineno(), "", "", "",
                        String(frb_lexer->YYText()));
         
             
          current_fn()->setParamArrayUsed(true);
          
          current_fn()->addParam(String($<param>3.str), $<param>3.type, $<vbool>2);
          free($<param>3.str);
      }
    | arg_mode arg /* [ByRef|ByVal] id As Type */
      {
      
          if(current_fn()->paramArrayUsed())
                frb_error->error(FrBErrors::FRB_ERR_PARAMARRAY_LAST,
                        FrBErrors::FRB_ERR_PARSE,
                        frb_lexer->lineno(), "", "", "",
                        String(frb_lexer->YYText()));
      
         if(current_fn()->firstOptionalParameter() > -1)
                frb_error->error(FrBErrors::FRB_ERR_OPT_PARAM,
                        FrBErrors::FRB_ERR_PARSE,
                        frb_lexer->lineno(), "", "", "",
                        String(frb_lexer->YYText()));
      
             
          current_fn()->addParam(String($<param>2.str), $<param>2.type, $<vbool>1);
          free($<param>2.str);
      }
    ;
          
    
arg_list: /* arg list for déclaration */
      arg_list FRB_KW_TOKEN_OP_LIST_SEP arg_type
    | arg_type
    ;
    
arg:
      FRB_IDENTIFIER as_type
      {
          $<param>$.str = $<str>1;
          $<param>$.type = $<vtype>2;
      }
    ;

    
array_expr_list: /* [expt, expr, ...] */
      FRB_KW_TOKEN_OP_ARRAY_SUB_BEGIN expr_list FRB_KW_TOKEN_OP_ARRAY_SUB_END
    | FRB_KW_TOKEN_OP_ARRAY_SUB_BEGIN /* empty */ FRB_KW_TOKEN_OP_ARRAY_SUB_END
    ;

parent_expr_list: /* (expt, expr, ...) */
      FRB_KW_TOKEN_OP_O_BRACKET expr_list FRB_KW_TOKEN_OP_C_BRACKET { $<exprs>$ = $<exprs>2; }
    | FRB_KW_TOKEN_OP_O_BRACKET /* empty */ FRB_KW_TOKEN_OP_C_BRACKET { $<exprs>$ = new FrBExprList(); }
    ;
     
expr_list:
      expr_list FRB_KW_TOKEN_OP_LIST_SEP expr { $<exprs>$ = $<exprs>1; $<exprs>$->push_back($<expr>3); }
    | expr { $<exprs>$ = new FrBExprList(); $<exprs>$->push_back($<expr>1); }
    ;


data_member_attr:
      FRB_KW_TOKEN_SHARED   { $<vint>$ = SC_SHARED; }
    | /* empty */           { $<vint>$ = SC_DEFAULT; }
    ;


data_member: /* eg Public Shared var As String [:= "e"] */
      member_scope_attr data_member_attr FRB_IDENTIFIER as_type 
      {
	_current_field = new FrBCodeField($<vtype>4);
        //_current_field->setIndex(current_class()->fieldList()->size());
	_current_field->setName($<str>3);
	_current_field->setScope($<vint>1);
	_current_field->setShared($<vint>2 == SC_SHARED);
	free($<str>3);
	current_class()->addField(_current_field);
      }
      declare_init
      {
	  _current_field->setInitExpr($<expr>6);
	  _current_field = 0;
      }
    ;



as_type:
      FRB_KW_TOKEN_AS type /* As type */
      {
          $<vtype>$ = $<vtype>2;
      }
    ;

as_type_optionnal:
       as_type     { $<vtype>$ = $<vtype>1; }
     | /* empty */ { $<vtype>$ = 0; }
    ;
    
// type_single: /* "single" type, no array */
//       FRB_KW_TOKEN_CONST FRB_KW_TOKEN_OP_O_BRACKET expr FRB_KW_TOKEN_OP_C_BRACKET /* Const type dynamique */
//     | FRB_KW_TOKEN_CONST FRB_IDENTIFIER /* Const String, Int... */
//     | FRB_KW_TOKEN_OP_O_BRACKET expr FRB_KW_TOKEN_OP_C_BRACKET /* type dynamique */
//     | FRB_IDENTIFIER /* String, Int... */
//     ;
//     
// type: /* single type or array of single type */
//       type_single /* single_type */
//     | type FRB_KW_TOKEN_OP_ARRAY_SUB_BEGIN FRB_KW_TOKEN_OP_ARRAY_SUB_END /* type[][][]... (array) */
//     ;
// 
// type_new_array: /* type[expr][expr]... */
//      type_single FRB_KW_TOKEN_OP_ARRAY_SUB_BEGIN expr FRB_KW_TOKEN_OP_ARRAY_SUB_END
//      | type_new_array FRB_KW_TOKEN_OP_ARRAY_SUB_BEGIN expr FRB_KW_TOKEN_OP_ARRAY_SUB_END
//     ;
//     
// new_expr:
//       FRB_KW_TOKEN_OP_NEW type parent_expr_list /* New type(arg, arg, ...) */
//     | FRB_KW_TOKEN_OP_NEW type_new_array /* New Type[expr] */
//     ;

// type: /* single type or array of single type */
//       type_single /* single_type */
//     | type FRB_KW_TOKEN_OP_ARRAY_SUB_BEGIN FRB_KW_TOKEN_OP_ARRAY_SUB_END /* type[][][]... (array) */
//     ;
// 
// type_new_array: /* type[expr][expr]... */
//      type_single FRB_KW_TOKEN_OP_ARRAY_SUB_BEGIN expr FRB_KW_TOKEN_OP_ARRAY_SUB_END
//      | type_new_array FRB_KW_TOKEN_OP_ARRAY_SUB_BEGIN expr FRB_KW_TOKEN_OP_ARRAY_SUB_END
//     ;
//     
// new_expr:
//       FRB_KW_TOKEN_OP_NEW type parent_expr_list /* New type(arg, arg, ...) */
//     | FRB_KW_TOKEN_OP_NEW type_new_array /* New Type[expr] */
//     ;

// type: /* "single" type, no array */
//       FRB_KW_TOKEN_CONST FRB_IDENTIFIER /* Const String, Int... */
//       {
//           free($<str>1);
//           frb_assert2(false, "not yet implemented : const T");
//           //$<vtype>$ = new FrBStaticType($<str>2, true);
//           
//       }
//     | FRB_IDENTIFIER /* String, Int... */
//       {
//           //mettre des expr.expr ) la place des types
//           //on resoud tout apres le parsage terminé
//           $<vtype>$ = 0;//FrBClass::getClassFromString($<str>1);
//           free($<str>1);
//       }
//     ;


type: 
      full_type { $<vtype>$ = $<vtype>1; }
    | FRB_KW_TOKEN_CONST full_type {  frb_assert2(false, "not yet implemented : const T"); }
    ;


full_type: /* a.b.c.d.e.f ... */
      full_type FRB_KW_TOKEN_OP_MEMBER FRB_IDENTIFIER       /* expr.id */
      {
          $<vtype>$ = new FrBUnresolvedTypeExpr($<str>3, current_class(), $<vtype>1);
          free($<str>3);
      }
    | FRB_IDENTIFIER       /* id */
      {
          $<vtype>$ = new FrBUnresolvedTypeExpr($<str>1, current_class());
          free($<str>1);
      }
    | intro_id /* Class, Function, ...*/
      {
          $<vtype>$ = new FrBUnresolvedTypeExpr($<str>1, current_class());
      }
    ;
    
    
new_expr:
      FRB_KW_TOKEN_OP_NEW type parent_expr_list /* New type(arg, arg, ...) */
      { $<expr>$ = new FrBNewExpr($<vtype>2, $<exprs>3); }
    ;

expr:
      FRB_KW_TOKEN_OP_O_BRACKET expr FRB_KW_TOKEN_OP_C_BRACKET             /* ( expr ) */
      { $<expr>$ = $<expr>2; }
      
    | expr FRB_KW_TOKEN_OP_ASSIGN_REF expr           /* expr := expr */
      {
          $<expr>$ = new FrBRefAssignExpr($<expr>1, $<expr>3);
      } 
      
    | expr FRB_KW_TOKEN_OP_ASSIGN_VAL expr           /* expr = expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ASSIGN_VAL); }
      
    | expr FRB_KW_TOKEN_OP_ADD_ASSIGN expr          /* expr += expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD_ASSIGN); }
      
    | expr FRB_KW_TOKEN_OP_SUB_ASSIGN expr           /* expr -= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_SUB_ASSIGN); }
      
    | expr FRB_KW_TOKEN_OP_MUL_ASSIGN expr           /* expr *= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_MUL_ASSIGN); }
      
    | expr FRB_KW_TOKEN_OP_DIV_ASSIGN expr           /* expr /= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_DIV_ASSIGN); }
      
    | expr FRB_KW_TOKEN_OP_INT_DIV_ASSIGN expr   /* expr \= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_INT_DIV_ASSIGN); }
      
    | expr FRB_KW_TOKEN_OP_POW_ASSIGN expr           /* expr ^= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_POW_ASSIGN); }
      
    | expr FRB_KW_TOKEN_OP_SUB expr                       /* expr - expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_SUB); }
      
    | expr FRB_KW_TOKEN_OP_ADD expr                       /* expr + expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_MUL expr                       /* expr * expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_MUL); }
      
    | expr FRB_KW_TOKEN_OP_DIV expr                       /* expr / expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_DIV); }
      
    | expr FRB_KW_TOKEN_OP_INT_DIV expr                 /* expr \ expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_INT_DIV); }
      
    | expr FRB_KW_TOKEN_OP_CONCAT expr                 /* expr & expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_CONCAT); }
      
    | expr FRB_KW_TOKEN_OP_MOD expr                         /* expr Mod expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_MOD); }
      
    | expr FRB_KW_TOKEN_OP_POW expr                      /* expr ^ expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_POW); }
      
    | FRB_KW_TOKEN_OP_SUB expr %prec FRB_KW_TOKEN_OP_UNARY_MINUS      /* -expr */
      { $<expr>$ = new FrBUnaryOpExpr($<expr>2, FrBKeywords::FRB_KW_OP_SUB); }

    | FRB_KW_TOKEN_OP_CARD expr                                          /* #expr */
      { $<expr>$ = new FrBUnaryOpExpr($<expr>2, FrBKeywords::FRB_KW_OP_CARD); }

    | expr FRB_KW_TOKEN_OP_INCR                                         /* expr++       generates 30 shift/reduce conflicts*/
      { $<expr>$ = new FrBUnaryOpExpr($<expr>1, FrBKeywords::FRB_KW_OP_INCR); }

    | expr FRB_KW_TOKEN_OP_DECR                                          /* expr-- */
      { $<expr>$ = new FrBUnaryOpExpr($<expr>1, FrBKeywords::FRB_KW_OP_DECR); }
    
    | expr FRB_KW_TOKEN_OP_EQ expr                             /* expr == expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_EQ); }
      
    | expr FRB_KW_TOKEN_OP_NE expr                             /* expr <> expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_NE); }
      
    | expr FRB_KW_TOKEN_OP_LT expr                             /* expr < expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_LT); }
      
    | expr FRB_KW_TOKEN_OP_GT expr                             /* expr > expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_GT); }
      
    | expr FRB_KW_TOKEN_OP_LE expr                             /* expr <= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_LE); }
      
    | expr FRB_KW_TOKEN_OP_GE expr                             /* expr >= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_GE); }
      
//     //| expr FRB_KW_TOKEN_OP_BITW_LSHIFT expr                                /* expr << expr */
//     //| expr FRB_KW_TOKEN_OP_BITW_RSHIFT expr                                /* expr >> expr */
//     //op () et [] et IF THEN ELSE et (-> ?) et ++ et -- et New et Delete
    | expr FRB_KW_TOKEN_OP_LOG_AND expr                   /* expr And expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_LOG_AND); }
      
    | expr FRB_KW_TOKEN_OP_LOG_OR expr                 /* expr Or expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_LOG_OR); }
      
    | expr FRB_KW_TOKEN_OP_LOG_XOR expr                  /* expr Xor expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_LOG_XOR); }
      
    | FRB_KW_TOKEN_OP_LOG_NOT expr                      /* Not expr */
      { $<expr>$ = new FrBUnaryOpExpr($<expr>2, FrBKeywords::FRB_KW_OP_LOG_NOT); }
    
    | expr FRB_KW_TOKEN_OP_IS expr                           /* expr Is expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_IS); }

    | expr FRB_KW_TOKEN_OP_IN expr                           /* expr In expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_IN); }

//     | expr FRB_KW_TOKEN_OP_CLASS_TYPEOF expr               /* expr IsTypeOf expr  |definissable sur object ca peut etre sympa */
//     | expr FRB_KW_TOKEN_OP_CLASS_INSTANCEOF expr       /* expr IsInstanceOf expr  | monobj.isTypeOf(Type|Object)*/
//     | expr FRB_KW_TOKEN_OP_CLASS_INHERITS expr           /* expr Inherits expr */
    
//    | FRB_KW_TOKEN_OP_SIZEOF expr                          /* SizeOf expr */


   //| FRB_KW_TOKEN_OP_CAST FRB_KW_TOKEN_OP_LT expr FRB_KW_TOKEN_OP_GT FRB_KW_TOKEN_OP_O_BRACKET expr FRB_KW_TOKEN_OP_C_BRACKET  /* Cast<expr>(expr) */
    | FRB_KW_TOKEN_OP_CAST FRB_KW_TOKEN_OP_O_BRACKET expr  FRB_KW_TOKEN_OP_LIST_SEP expr FRB_KW_TOKEN_OP_C_BRACKET  /* Cast(expr, expr) */
      { $<expr>$ = new FrBCastExpr($<expr>3, $<expr>5);  }
    
    | expr FRB_KW_TOKEN_OP_MEMBER FRB_KW_TOKEN_OPERATOR operator_overloadable 
      {

      }
      
    | new_expr                            /* New */
      { $<expr>$ = $<expr>1; }
    | expr FRB_KW_TOKEN_OP_MEMBER FRB_IDENTIFIER                     /* expr.expr_id */
      {
          //$<expr>$ = new FrBMemberOpExpr($<expr>1,  new FrBUnresolvedIdExpr($<str>3));
          $<expr>$ = new FrBUnresolvedIdWithContextExpr($<expr>1, $<str>3);
          free($<str>3);
      }
    | expr parent_expr_list              /* function call expr(expr, expr, ...) */
      {
          $<expr>$ = new FrBFunctionCallExpr($<expr>1, $<exprs>2);
      }
      
    | expr array_expr_list                /* array sub expr[expr, expr...] */
    | literal_expr
    ;

literal_bool:
      FRB_KW_TOKEN_TRUE { $<expr>$ = new FrBBoolExpr(true); }
    | FRB_KW_TOKEN_FALSE { $<expr>$ = new FrBBoolExpr(false); }
    ;

literal_expr:
      FRB_TYPE_LITERAL_DOUBLE
    | FRB_TYPE_LITERAL_SINGLE
    | FRB_TYPE_LITERAL_INT            { $<expr>$ = new FrBIntExpr($<vint>1); }
    | FRB_TYPE_LITERAL_LONGINT
    | FRB_TYPE_LITERAL_SHORTINT
    | FRB_TYPE_LITERAL_STRING         { $<expr>$ = new FrBStringExpr($<str>1); }
    | FRB_TYPE_LITERAL_CHAR
    | literal_bool { $<expr>$ = $<expr>1; }
    | FRB_KW_TOKEN_NULL { $<expr>$ = new FrBNullExpr(); /*FrBNullExpr::nullExpr();*/ }
    | FRB_KW_TOKEN_ME { $<expr>$ = new_me_expr(); }  
    | identifier_expr { $<expr>$ = $<expr>1; }
    | array
    ;

identifier_expr:
      FRB_IDENTIFIER                  
      {
      
            String name($<str>1);
            free($<str>1);

            if(!fn_stack.empty())
            {
                FrBCodeFunction * cf = current_fn();


                /* local var */
                int idvar = cf->findLocalVar(name);
                if(idvar > -1)
                {
                    $<expr>$ = new FrBLocalVarExpr(cf, cf->getLocalVar(idvar), -idvar - 1);
                    break;
                }

                /* parameter */
                idvar = cf->getParam(name);
                if(idvar > -1)
                {
                    $<expr>$ = new FrBLocalVarExpr(cf, cf->getURParam(idvar),  idvar + 1);
                    break;
                }
            
            }

            $<expr>$ = new FrBUnresolvedIdWithContextExpr(new_me_expr(true), name);
          
      }
    | intro_id
      {
	  $<expr>$ = new FrBUnresolvedIdWithContextExpr(new_me_expr(true), $<str>1);
      }
    ;

/* identifier for introspection */
intro_id:
      FRB_KW_TOKEN_CLASS      { $<str>$ = $<str>1; }
   |  FRB_KW_TOKEN_FUNCTION   { $<str>$ = $<str>1; }
   ;

array: /* {expr, expr, ...} */
      FRB_KW_TOKEN_OP_ARRAY_INI_BEGIN array_elt_list FRB_KW_TOKEN_OP_ARRAY_INI_END
    | FRB_KW_TOKEN_OP_ARRAY_INI_BEGIN /* empty */ FRB_KW_TOKEN_OP_ARRAY_INI_END
    ;

array_elt_list:
      array_elt_list FRB_KW_TOKEN_OP_LIST_SEP expr
    | expr
    ;

for_loop_step: /* Step expr */
      FRB_KW_TOKEN_STEP expr { $<expr>$ = $<expr>2; }
    | /* nothing */ { $<expr>$ = 0; }
    ;

for_increment_direction: /* To or DownTo */
      FRB_KW_TOKEN_TO { $<vint>$ = 1; }
    | FRB_KW_TOKEN_DOWNTO { $<vint>$ = -1; }
    ;


for_loop: /* For id := expr To|DownTo expr [Step expr] */
      FRB_KW_TOKEN_FOR /*1*/ identifier_expr /*2*/ declare_init_nonempty /*3*/
                                 for_increment_direction /*4*/ expr /*5*/ for_loop_step /*6*/ new_line /*7*/
     {
        FrBForLoopStatement * b = new FrBForLoopStatement($<expr>2, $<expr>3, $<vint>4, $<expr>5, $<expr>6);

	current_block()->addStat(b);

	block_stack.push(b);

      } /*8*/
      function_content_list /*9*/
      {
	  block_stack.pop();
      } /*10*/
      FRB_KW_TOKEN_NEXT /*11*/
    ;

//for_loop_dec: /* For id As Type := expr To|DownTo expr [Step expr] */
//      FRB_KW_TOKEN_FOR /*1*/ identifier_expr /*2*/ as_type /*3*/ declare_init_nonempty /*4*/
//                                 for_increment_direction /*5*/ expr /*6*/ for_loop_step /*7*/ new_line /*8*/
//     {
//	current_block()->addStat(new FrBDeclareStatement(current_fn(), 1, $<vtype>3));
//
//        FrBForLoopStatement * b = new FrBForLoopStatement($<expr>2, $<expr>4, $<vint>5, $<expr>6, $<expr>7);
//
//	block_stack.push(b);
//
//      } /*9*/
//      function_content_list /*10*/
//      {
//	  block_stack.pop();
//      } /*11*/
//      FRB_KW_TOKEN_NEXT /*12*/
//    ;


    //TODO : pour le for avec déclaration (ie <=> for(int i = 0; ....)) faire une
    //       forme sépciale qui déclare directement For i As Int = 0

/*         FrBDeclareStatement * d = new FrBDeclareStatement(fn, id_list.size(),
                                                                $<vtype>4, $<expr>5);
            fn->addStat(d);

            for(CStringList::iterator it = id_list.begin(); it != id_list.end(); ++it)
            {
                int var_count = fn->localVarCount();

                d->addVarID(-var_count - 1);

                fn->addLocalVar((*it), $<vtype>4);

                if(fn->localVarCount() == var_count || fn->getParam(*it) != -1)
                    frb_error->error(FrBErrors::FRB_ERR_REDECL_ID,
                    FrBErrors::FRB_ERR_SEMANTIC,
                    frb_lexer->lineno(), "", "", "",
                    String(*it));

                free(*it);
		}*/

