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


#include "syntaxer.h"
#include "frbsyntaxer.h"
#include "frblexer.h"


//#define TOKEN(x) FrBSyntaxicalParser::x

#define KW_TOKEN_SHIFT (TOKEN(FRB_KW_TOKEN___BEGIN) + 1)
    /*décalage entre les token et les kw pr un mm mot clef
     les kw commencent à 0 les token pour yacc à FRB_KW_TOKEN___BEGIN fixé par yacc */

//recupère un token du syntaxer
#define TOKEN(x) FrBSyntaxicalParser::x
//converti un code mot clef en token pour le syntaxer
#define KW2TOKEN(x) (x + KW_TOKEN_SHIFT)

//yylval
#define YYLVAL (frb_syntaxer->yylval)


#include <cstdlib>

/*
    Les mots clefs déclarent une unique fois dans frbkeywords.key, les
    constantes suivantes sont automatiquement définies lors du make :
    
    FRB_KW_TOKEN_* correspond au token des mot clefs FRB_KW_*
                   
    FRB_KW_*       correspond au mot clefs declarés dans FrBKeywords
                   (utilisés dans le lexer)
*/

//distinguer les doubles des float
//possibilité de renter un nombre en octal, decimal, hexadécimal, binaire

//il faudrait deux tableaux un pr les kw et pr les op
//comme ca ca sera plus rapide

/* pr les operator, on inclus les mots clef comme ds le fichier yacc (par %DECLARE_SYMBOL_RULES) */


//faire les frb_string/char/date/time/datetime

/*


str : p174

*/

#define FRB_LEX_ERR(err)  frb_error->error(err, \
                                      FrBErrors::FRB_ERR_LEXE, \
                                      yylineno, currentFile(), "", "", String(yytext))


                                      
   /* FrBErrorDescriptor(int errno, int contexterrno, int line, const String& file, 
                        const String& classname, const String& function, const String& token)*/
                         
//#define DBG_PRINTF(args ...) printf(args)     
#define DBG_PRINTF(args ...)   
%}


%pointer
%option noyywrap
%option c++
%option yyclass="FrBFlexLexerEx"
%option yylineno

frb_blank             ([ \t\v])
frb_blanks_nl         ({frb_blanks}|"\n")
frb_newline           ([\n\r])
frb_digit             ([0-9])
frb_integer           ({frb_digit}+)
frb_longint           ("&L"{frb_integer})
frb_shortint          ("&S"{frb_integer})
frb_double            ({frb_integer}("."{frb_integer})?([eE][+-]?{frb_integer})?)
frb_single            ("&F"{frb_double})
frb_hexa              ("&H"[0-9A-Fa-f]+)
frb_bin               ("&B"[01]+)
frb_octal             ("&O"[0-7]+)
frb_cplxp             ({frb_double}?"i")
frb_identifier        ([_a-zA-Z]([_a-zA-Z0-9]*))
frb_keyword           ([A-Z]([a-zA-Z]*))
frb_import_lib        ({frb_identifier}(\.{frb_identifier})*(\.\*)?)
frb_import_local      (\"[^\"\r\n]*\")
frb_deflang           ("DefLang")

%x import_mode defset_mode comment_mode string_mode


%%

\"[^"\n]*\"                           { 
                                        if(yytext[ yyleng - 2 ] == '\\')
                                        {
                                            yyless(yyleng - 1);
                                            yymore();
                                        }
                                        else
                                        {
                                            yytext[ yyleng - 1 ] = 0;
                                            YYLVAL.str = ++yytext;
                                            return TOKEN(FRB_TYPE_LITERAL_STRING);
                                        }
                                    }
                                    
\"[^"\n]*\n                         { 
                                        yytext[ yyleng - 1 ] = 0;
                                        FRB_LEX_ERR(FrBErrors::FRB_ERR_STRING);
                                        
                                        return TOKEN(FRB_TYPE_LITERAL_STRING);
                                    }

<*>"'"                              { DBG_PRINTF("CO\n"); BEGIN(comment_mode); }
<comment_mode>{frb_newline}         { DBG_PRINTF("NL\n"); BEGIN(0); return TOKEN(FRB_END_LINE); }
<comment_mode>.                     { /* on mange les commentaires */ }

({frb_blank}+)"_\n"({frb_blank}*)   { /* on mange les _\n */ }

<*>{frb_blank}*                     {
                                        /* <*>({frb_blank}*)|(_{frb_newline})   */
                                    }
                                    

{frb_deflang}                        {
                                            BEGIN(defset_mode);
                                            break;
                                     }

<defset_mode>{frb_identifier}       {
                                        int newSet = FrBKeywords::findSet(yytext);
                                    
                                        if( newSet != FrBKeywords::FRB_SET__NOTFOUND )
                                        {
                                            FrBKeywords::setCurrentSet(newSet);
                                            DBG_PRINTF("Changement de set : %s (%d)\n", yytext, newSet);
                                        }
                                        else
                                        {
                                            FRB_LEX_ERR(FrBErrors::FRB_ERR_LANG);
                                            //exit(1);
                                        }
                                        BEGIN(0);
                                    }
                                    
<import_mode>{frb_import_lib}       {
                                        DBG_PRINTF("Importer (lib) : %s\n", yytext);
                                        BEGIN(0);
                                    }
                                    
<import_mode>{frb_import_local}     {
                                        DBG_PRINTF("Importer (local) : %s\n", yytext);
                                        BEGIN(0);
                                    }

%DECLARE_SYMBOL_RULES
        
                        
{frb_keyword}                       { /* mot clef ou id */
                                       /*if( yytext[yyleng-1] == '\n' )
                                          unput('\n');*/

                                       //printf("yytext=%s, GET_KEYWORD(yytext)=%d\n", yytext, GET_KEYWORD(yytext));
                                       
                                       int keyw_id = FrBKeywords::findKeyword(yytext);
                                       switch(keyw_id)
                                       {
                                       case FrBKeywords::FRB_KW__NOTFOUND:
                                            {
                                                DBG_PRINTF("Identifier : %s\n", yytext);
                                                first_instruction = false;
                                                YYLVAL.str = strdup(yytext);
                                                return TOKEN(FRB_IDENTIFIER);
                                            }
                                       case FrBKeywords::FRB_KW_IMPORT:
                                            {
                                                if(first_instruction)
                                                    BEGIN(import_mode);
                                                else
                                                    FRB_LEX_ERR(FrBErrors::FRB_ERR_IMPORT);
                                                break;
                                            }
                                       default:
                                            {
                                                //on utilise pas yytext car il faudrait faire un strdup
                                                // et on a deja les chaines en mémoire
                                                YYLVAL.str = const_cast<CString>(FrBKeywords::getKeyword(keyw_id));
                                                DBG_PRINTF("Keyword : %s (%d, %d) {str=%s}\n", yytext, keyw_id, KW2TOKEN(keyw_id), YYLVAL.str);
                                                first_instruction = false;
                                                //YYLVAL.a = 0;
                                                return KW2TOKEN(keyw_id);
                                            }
                                       }

                                    }

{frb_identifier}                    {
                                       /* identifier */
                                       /*if( yytext[yyleng-1] == '\n' )
                                          unput('\n');*/
                                          
                                       DBG_PRINTF("Identifier : %s\n", yytext);
                                       first_instruction = false; 
                                       YYLVAL.str = strdup(yytext);
                                       return TOKEN(FRB_IDENTIFIER);
                                    }


{frb_newline}        {
                        DBG_PRINTF("NL\n");
                        return TOKEN(FRB_END_LINE);
                     }


{frb_integer}        { DBG_PRINTF("integer = %s\n", yytext); 
                            YYLVAL.vint = atoi(yytext); return TOKEN(FRB_TYPE_LITERAL_INT); }
{frb_double}         { return TOKEN(FRB_TYPE_LITERAL_DOUBLE); }
{frb_single}         { return TOKEN(FRB_TYPE_LITERAL_SINGLE); }
{frb_longint}        { return TOKEN(FRB_TYPE_LITERAL_LONGINT); }
{frb_shortint}       { return TOKEN(FRB_TYPE_LITERAL_SHORTINT); }


<<EOF>>    { DBG_PRINTF("EOF\n"); return TOKEN(FRB_END_OF_FILE); }


<*>. { 
        FRB_LEX_ERR(FrBErrors::FRB_ERR_UNEXPECTED_TOKEN);
        
        return 0;
     }





