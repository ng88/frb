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


#ifndef FRBSYNTAXER_H
#define FRBSYNTAXER_H


#ifndef YY_FrBSyntaxicalParser_MEMBERS
#define YY_FrBSyntaxicalParser_MEMBERS \
     public: \
        FrBFlexLexerEx * frb_lexer; \
        FrBClassMap * frb_classes; \
        FrBErrorCollector * frb_error; \
     private: \
         FrBIfStatement * current_if;\
         FrBCodeClassStack class_stack; \
         FrBCodeFunctionStack fn_stack; \
         FrBBlockStack block_stack; \
         CStringList id_list; \
         inline FrBCodeClass* current_class() \
         { \
             frb_assert2(!class_stack.empty(), "frbsyntaxer.h::FrBSynater::current_class()");  \
             return class_stack.top(); \
         } \
         inline FrBCodeFunction* current_fn() \
         { \
             if(fn_stack.empty()) \
               frb_error->error(FrBErrors::FRB_ERR_INVALID_OUTSIDE_EXPR, \
                                FrBErrors::FRB_ERR_SEMANTIC, \
                                frb_lexer->lineno(), "", "", "", \
                                String(frb_lexer->YYText()) ); \
             \
             /*frb_assert2(!fn_stack.empty(), "frbsyntaxer.h::FrBSynater::current_fn()");*/ \
             return fn_stack.top(); \
         } \
         inline FrBStatementBlock* current_block() \
         { \
             frb_assert2(!block_stack.empty(), "frbsyntaxer.h::FrBSynater::current_block()");  \
             return block_stack.top(); \
         } \
         inline FrBMeExpr* new_me_expr() \
         { \
            if(fn_stack.empty()) \
                return new FrBOutsideMeExpr(current_class()); \
            else \
            { \
                if(current_fn()->shared()) \
                    ;\
                return new FrBInsideMeExpr(current_fn()); \
            } \
         } \
     public: \
        virtual ~FrBSyntaxicalParser() {} \
        void switch_lexer_streams( std::istream* new_in = 0, std::ostream* new_out = 0 )\
        { \
            frb_lexer->switch_streams(new_in, new_out); \
        } \
        \


#endif

#ifndef YY_FrBSyntaxicalParser_ERROR_BODY
#define YY_FrBSyntaxicalParser_ERROR_BODY \
        {  \
            FrBErrorDescriptor err = FrBErrorDescriptor::fromYYError(msg, frb_lexer->YYText()); \
             \
            err.setFileName(""); \
            err.setClassName(""); \
            err.setFunctionName(""); \
            err.setLine(frb_lexer->lineno()); \
     \
            frb_error->error(err);\
        }
#endif


#ifndef  YY_FrBSyntaxicalParser_LEX_BODY
#define YY_FrBSyntaxicalParser_LEX_BODY { return frb_lexer->yylex(); }
#endif

#endif
