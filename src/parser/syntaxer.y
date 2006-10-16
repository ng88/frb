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
    FrBClass * type;
    char *     str;
};

struct FnAttr
{
    int   scope;
    int   class_attr;
    bool  is_const;
};


%}

%name FrBSyntaxicalParser

%union
{
    char *                     str;
    FrBBaseObject *            obj;
    FrBExpr *                  expr;
    FrBClass *                 vtype;
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

/* 2. Opérateurs de comparaison */
%left FRB_KW_TOKEN_OP_EQ FRB_KW_TOKEN_OP_NE FRB_KW_TOKEN_OP_LT FRB_KW_TOKEN_OP_GT FRB_KW_TOKEN_OP_LE FRB_KW_TOKEN_OP_GE

/* 3. Opérateurs logiques */
%left FRB_KW_TOKEN_OP_LOG_AND FRB_KW_TOKEN_OP_LOG_OR FRB_KW_TOKEN_OP_LOG_XOR FRB_KW_TOKEN_OP_LOG_NOT

/* 4. Opérateurs d'assignement */
%right FRB_KW_TOKEN_OP_ASSIGN_VAL FRB_KW_TOKEN_OP_ASSIGN_REF FRB_KW_TOKEN_OP_ADD_ASSIGN FRB_KW_TOKEN_OP_SUB_ASSIGN FRB_KW_TOKEN_OP_MUL_ASSIGN FRB_KW_TOKEN_OP_DIV_ASSIGN FRB_KW_TOKEN_OP_INT_DIV_ASSIGN FRB_KW_TOKEN_OP_POW_ASSIGN

/* 5. Opérateurs arithmétiques */
%left FRB_KW_TOKEN_OP_ADD FRB_KW_TOKEN_OP_SUB
%left FRB_KW_TOKEN_OP_MOD
%left FRB_KW_TOKEN_OP_DIV FRB_KW_TOKEN_OP_MUL FRB_KW_TOKEN_OP_INT_DIV
%right FRB_KW_TOKEN_OP_POW

/* 6. Opérateur membre */
%left FRB_KW_TOKEN_OP_MEMBER

/* 7. Opérateur d'objet */
%left FRB_KW_TOKEN_OP_IS FRB_KW_TOKEN_OP_TYPEOF FRB_KW_TOKEN_OP_INSTANCEOF FRB_KW_TOKEN_OP_INHERITS
%nonassoc FRB_KW_TOKEN_OP_SIZEOF

%nonassoc FRB_KW_TOKEN_OP_UNARY_MINUS FRB_KW_TOKEN_OP_O_BRACKET FRB_KW_TOKEN_OP_C_BRACKET FRB_KW_TOKEN_OP_ARRAY_SUB_BEGIN FRB_KW_TOKEN_OP_ARRAY_SUB_END

%%

/* 10 shift/reduce a cause du empty dans member_scope_attr */

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
    //| FRB_KW_TOKEN_VIRTUAL
    | FRB_KW_TOKEN_PUREVIRTUAL  { $<vint>$ = SC_ABSTRACT; }
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

function_return_type:
       as_type     { $<vtype>$ = $<vtype>1; }
     | /* empty */ { $<vtype>$ = 0; }
 
function:
      fn_attr /*1*/ function_type /*2*/ sub_name /*3*/
      { /*4*/
           frb_assert2(!class_stack.empty(), "sub declaration");
           
           FrBCodeFunction * nfn = new FrBCodeFunction();
           
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
                   current_class()->addConstructor(nfn);
               }
               else if( $<str>3 == FrBKeywords::getKeyword(FrBKeywords::FRB_KW_DESTRUCTOR_NAME))
               {
                   current_class()->addDestructor(nfn);
               }
               else
               {
                   current_class()->addFunction(nfn);
                   free($<str>3);
               }
                   
               
           }
           else
           {
               frb_assert2(false, "inner functions not implemented");
               free($<str>3);
           }
               
           fn_stack.push(nfn);
           
      }
      fn_arg_list /*5*/ function_return_type /*6*/ /* sub/function */
      { /*7*/
           if(current_fn()->sub() && $<vtype>6 != 0)
                frb_error->error(FrBErrors::FRB_ERR_SUB_RETURN_TYPE,
                    FrBErrors::FRB_ERR_PARSE,
                    frb_lexer->lineno(), "", "", "",
                    String(frb_lexer->YYText()));
            else if(!current_fn()->sub() && $<vtype>6 == 0)
                frb_error->error(FrBErrors::FRB_ERR_FN_RETURN_TYPE,
                    FrBErrors::FRB_ERR_PARSE,
                    frb_lexer->lineno(), "", "", "",
                    String(frb_lexer->YYText()));
                    
            current_fn()->setReturnType($<vtype>6);        
           
      }
      function_content_list /*8*/
      FRB_KW_TOKEN_END /*9*/ function_type /*10*/
      { /*11*/   
          if($<str>2 != $<str>10)
                frb_error->error(FrBErrors::FRB_ERR_FN_DEC_END,
                    FrBErrors::FRB_ERR_PARSE,
                    frb_lexer->lineno(), "", "", "",
                    String(frb_lexer->YYText()));
           
          fn_stack.pop();
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
     
fn_stat:
      expr new_line /* expression */
      {
          current_fn()->addStat(new FrBExprStatement($<expr>1));
      }
    | dim_stat new_line /* déclaration */
    | delete_stat new_line /* destruction */
    | return_stat new_line /* return  */
    | typedef_stat /* typedef */
    ;
    

dim_stat: /* Dim nom1_1, nom1_2, ... As type [= init], nom2_1, nom2_2, ... As type [= init], ... */
      FRB_KW_TOKEN_DECLARE declare_list
    ;

declare_init:
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
    | /* empty */                     { $<expr>$ = 0; }

declare_list: /* nom1_1, nom1_2, ... As type [= init], nom2_1, nom2_2, ... As type [= init], ... */
      declare_list FRB_KW_TOKEN_OP_LIST_SEP identifier_list as_type declare_init
      {
            FrBCodeFunction * fn = current_fn();
            for(CStringList::iterator it = id_list.begin(); it != id_list.end(); ++it)
            {
                int var_count = fn->localVarCount();

                if($<expr>5 != 0 && !FrBClass::areCompatibles($<vtype>4, $<expr>5->getClass()))
                    frb_error->error(FrBErrors::FRB_ERR_TYPE_MISMATCH,
                    FrBErrors::FRB_ERR_SEMANTIC,
                    frb_lexer->lineno(), "", "", "",
                    String(*it));
                
                fn->addStat( new FrBDeclareStatement(var_count, $<vtype>4, $<expr>5) );
                fn->addLocalVar((*it), $<vtype>4);
                
                if(fn->localVarCount() == var_count)
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
            FrBCodeFunction * fn = current_fn();
            for(CStringList::iterator it = id_list.begin(); it != id_list.end(); ++it)
            {
                int var_count = fn->localVarCount();

                if($<expr>3 != 0 && !FrBClass::areCompatibles($<vtype>2, $<expr>3->getClass()))
                    frb_error->error(FrBErrors::FRB_ERR_TYPE_MISMATCH,
                    FrBErrors::FRB_ERR_SEMANTIC,
                    frb_lexer->lineno(), "", "", "",
                    String(*it));
                
                fn->addStat( new FrBDeclareStatement(var_count, $<vtype>2, $<expr>3) );
                fn->addLocalVar((*it), $<vtype>2);

                if(fn->localVarCount() == var_count)
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



data_member:
      FRB_KW_TOKEN_PRIVATE
    ;



as_type:
      FRB_KW_TOKEN_AS type /* As type */
      {
          $<vtype>$ = $<vtype>2;
      }
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

type: /* "single" type, no array */
      FRB_KW_TOKEN_CONST FRB_IDENTIFIER /* Const String, Int... */
      {
          free($<str>1);
          frb_assert2(false, "not yet implemented : const T");
          //$<vtype>$ = new FrBStaticType($<str>2, true);
          
      }
    | FRB_IDENTIFIER /* String, Int... */
      {
          //TODONEXT trouver une soluce pour ca 
          //mettre des expr.expr ) la place des types
          //on resoud tout apres le parsage terminé
          $<vtype>$ = 0;//FrBClass::getClassFromString($<str>1);
          free($<str>1);
      }
    ;
    
new_expr:
      FRB_KW_TOKEN_OP_NEW type parent_expr_list /* New type(arg, arg, ...) */
      { /*$<expr>$ = new FrBNewExpr($<vtype>2, $<exprs>3);*/ }
    ;

 /* name expr, eg: Class1, String, Module1.Class1, ... */
name_expr:
      name_expr FRB_KW_TOKEN_OP_MEMBER FRB_IDENTIFIER                     /* expr.expr_id */
      {
          //$<expr>$ = new FrBMemberOpExpr($<expr>1, String($<str>3));
          free($<str>3);
      }
    | FRB_IDENTIFIER
      {
          //$<expr>$ = new FrBMemberOpExpr($<expr>1, String($<str>3));
          free($<str>1);
      }
    ;
    


expr:
      FRB_KW_TOKEN_OP_O_BRACKET expr FRB_KW_TOKEN_OP_C_BRACKET             /* ( expr ) */
      { $<expr>$ = $<expr>2; }
      
    | expr FRB_KW_TOKEN_OP_ASSIGN_REF expr           /* expr := expr */
      { $<expr>$ = $<expr>1 = $<expr>3; } //TODO je pense pas que ca marche comme ca 
      
    | expr FRB_KW_TOKEN_OP_ASSIGN_VAL expr           /* expr = expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ASSIGN_VAL); }
      
    | expr FRB_KW_TOKEN_OP_ADD_ASSIGN expr          /* expr += expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_SUB_ASSIGN expr           /* expr -= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_MUL_ASSIGN expr           /* expr *= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_DIV_ASSIGN expr           /* expr /= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_INT_DIV_ASSIGN expr   /* expr \= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_POW_ASSIGN expr           /* expr ^= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_SUB expr                       /* expr - expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_SUB); }
      
    | expr FRB_KW_TOKEN_OP_ADD expr                       /* expr + expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_MUL expr                       /* expr * expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_MUL); }
      
    | expr FRB_KW_TOKEN_OP_DIV expr                       /* expr / expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_DIV); }
      
    | expr FRB_KW_TOKEN_OP_INT_DIV expr                 /* expr \ expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_CONCAT expr                 /* expr & expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_CONCAT); }
      
    | expr FRB_KW_TOKEN_OP_MOD expr                         /* expr Mod expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_POW expr                      /* expr ^ expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | FRB_KW_TOKEN_OP_SUB expr %prec FRB_KW_TOKEN_OP_UNARY_MINUS      /* -expr */
    
    | expr FRB_KW_TOKEN_OP_EQ expr                             /* expr == expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_NE expr                             /* expr <> expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_LT expr                             /* expr < expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_GT expr                             /* expr > expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_LE expr                             /* expr <= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_GE expr                             /* expr >= expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
//     //| expr FRB_KW_TOKEN_OP_BITW_LSHIFT expr                                /* expr << expr */
//     //| expr FRB_KW_TOKEN_OP_BITW_RSHIFT expr                                /* expr >> expr */
//     //op () et [] et IF THEN ELSE et (-> ?) et ++ et -- et New et Delete
    | expr FRB_KW_TOKEN_OP_LOG_AND expr                   /* expr And expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_LOG_OR expr                 /* expr Or expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | expr FRB_KW_TOKEN_OP_LOG_XOR expr                  /* expr Xor expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }
      
    | FRB_KW_TOKEN_OP_LOG_NOT expr                      /* Not expr */
    
    | expr FRB_KW_TOKEN_OP_IS expr                           /* expr Is expr */
      { $<expr>$ = new FrBBinOpExpr($<expr>1, $<expr>3, FrBKeywords::FRB_KW_OP_ADD); }

//     | expr FRB_KW_TOKEN_OP_CLASS_TYPEOF expr               /* expr IsTypeOf expr */
//     | expr FRB_KW_TOKEN_OP_CLASS_INSTANCEOF expr       /* expr IsInstanceOf expr */
//     | expr FRB_KW_TOKEN_OP_CLASS_INHERITS expr           /* expr Inherits expr */
    
    | FRB_KW_TOKEN_OP_SIZEOF expr                          /* SizeOf expr */
    
    | expr FRB_KW_TOKEN_OP_MEMBER FRB_KW_TOKEN_OPERATOR operator_overloadable 
      {

      }
      
    | new_expr                            /* New */
    | expr FRB_KW_TOKEN_OP_MEMBER FRB_IDENTIFIER                     /* expr.expr_id */
      {
          //$<expr>$ = new FrBMemberOpExpr($<expr>1, String($<str>3));
          free($<str>3);
      }
    | expr parent_expr_list              /* function call expr(expr, expr, ...) */
      {
          //$<expr>1
          //$<exprs>2
      }
      
    | expr array_expr_list                /* array sub expr[expr, expr...] */
    | literal_expr
    ;


literal_expr:
      FRB_TYPE_LITERAL_DOUBLE
    | FRB_TYPE_LITERAL_SINGLE
    | FRB_TYPE_LITERAL_INT            { $<expr>$ = new FrBIntExpr($<vint>1); }
    | FRB_TYPE_LITERAL_LONGINT
    | FRB_TYPE_LITERAL_SHORTINT
    | FRB_TYPE_LITERAL_STRING         { $<expr>$ = new FrBStringExpr($<str>1); }
    | FRB_TYPE_LITERAL_CHAR
    | FRB_KW_TOKEN_NULL
    | FRB_KW_TOKEN_ME { $<expr>$ = new FrBLocalVarExpr(current_class(), current_fn()->localVarCount() + 1); }
    | FRB_IDENTIFIER                  
      {
      
            String name($<str>1);
            printf("ID NAME %s\n", $<str>1);
            free($<str>1);
            
            const FrBClass * cc = current_class();
            const FrBCodeFunction * cf = current_fn();
            
            (void)cc;
            
            
            /*
                We look for:
                  X 1. local var
                  X 2. function parameter
                    3. local class member
                 ---
                  X 4. local class function/sub
                    5. local class property
                  X 6. class names of the inners classes
                    7. class names of the outer class
                  X 8. imported class name
            
            */
            
            //TODO ici on resoud que les var local(param, this, var) pas le reste (post traitement)
            
            /* 1. local var */
           
            int idvar = cf->findLocalVar(name);
            if(idvar > -1)
            {
                /* found */
                
                $<expr>$ = new FrBLocalVarExpr(cf->getLocalVar(idvar), idvar);
                
                puts("ID FOUND -- /* 1. local var */\n");
                
                break;
            }
            
            /* 2. function parameter */
            idvar = cf->getParam(name);
            
            if(idvar > -1)
            {
                /* found */
                
                $<expr>$ = new FrBLocalVarExpr(cf->parameterType(idvar),  cf->localVarCount() + 1 + idvar);
                
                puts("ID FOUND -- /* 2. function parameter */\n");
                break;
            }
            
            puts("ID UNRESOLVED\n");
            
            $<expr>$ = new FrBUnresolvedIdExpr(name);
            
//             /* 4. local class function/sub */
//             FrBClass::FnPairIt pit = cc->findFunctions(name);
//             
//             if(pit.second->second)
//             {
//                 /* ambiguity */
// 
//                 puts("ID AMBIGUITY -- /* 4. local class function/sub */\n");
//                 exit(0);
//                 
//                 break;
//             }
//             else if(pit.first != cc->functionList()->end())
//             {
//                 /* found */
//                 
//                 $<expr>$ = new FrBObjectIdExpr(new FrBFunctionWrapper( pit.first->second ));
//                 
//                 puts("ID FOUND -- /* 4. local class function/sub */\n");
//                 
//                 break;
//             }
//             
//             /* 6. class names of the inners classes */
//             FrBClass::ClassContainer::const_iterator iit = cc->innerClassList()->find(name);
//             
//             if(iit != cc->innerClassList()->end())
//             {
//                 /* found */
//                 
//                 $<expr>$ = new FrBObjectIdExpr(new FrBClassWrapper( iit->second ));
//                 
//                 puts("ID FOUND --  /* 6. class names of the inners classes */\n");
// 
//                 break;
//             }
//             
//             
//             /* 8. imported class name */
//             const FrBClass * c = FrBClass::getClassFromString(name);
//             
//             if(c)
//             {
//                 /* found */
//                 
//                 $<expr>$ = new FrBObjectIdExpr(new FrBClassWrapper(c));
//                 
//                 puts("ID FOUND --  /* 8. imported class name */\n");
//                 
//                 break;
//             }
          
      }
    | FRB_KW_TOKEN_NOTHING
    | array
    ;


array: /* {expr, expr, ...} */
      FRB_KW_TOKEN_OP_ARRAY_INI_BEGIN array_elt_list FRB_KW_TOKEN_OP_ARRAY_INI_END
    | FRB_KW_TOKEN_OP_ARRAY_INI_BEGIN /* empty */ FRB_KW_TOKEN_OP_ARRAY_INI_END
    ;

array_elt_list:
      array_elt_list FRB_KW_TOKEN_OP_LIST_SEP expr
    | expr
    ;


    //TODO : pour le for avec déclaration (ie <=> for(int i = 0; ....)) faire une
    //       forme sépciale qui déclare directement For i As Int = 0

