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


#ifndef FRBPARSINGTREE_H
#define FRBPARSINGTREE_H

#include <vector>

#ifndef YY_FrBSyntaxicalParser_MEMBERS
#define YY_FrBSyntaxicalParser_MEMBERS \
     private: \
        FrBFlexLexer frb_lexer; \
     public: \
        virtual ~FrBSyntaxicalParser() {} \
        void switch_lexer_streams( std::istream* new_in = 0, std::ostream* new_out = 0 )\
        { \
            frb_lexer.switch_streams(new_in, new_out); \
        }
#endif
        

#ifndef YY_FrBSyntaxicalParser_ERROR_BODY
#define YY_FrBSyntaxicalParser_ERROR_BODY \
    { \
        fprintf(stderr, "Error: %s\n", msg); \
    }
#endif

#ifndef YY_FrBSyntaxicalParser_LEX_BODY
#define YY_FrBSyntaxicalParser_LEX_BODY { return frb_lexer.yylex(); }
#endif

struct FrBParseNode;

typedef std::vector<FrBParseNode*> FrBNodeList;

enum FrBNodeType
{
};


struct FrBParseNode
{
    FrBNodeList childs;
    FrBNodeType type;
};



#endif
