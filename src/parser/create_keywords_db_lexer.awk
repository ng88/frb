###########################################################################
#   This file is part of the FrB project                                  #
#                                                                         #
#   FrB project                                                           #
#                                                                         #
#   Copyright (C) 2006 by GUILLAUME Nicolas                               #
#   ng@ngsoft-fr.com                                                      #
#                                                                         #
#   http://frb.ngsoft-fr.com/                                             #
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   (at your option) any later version.                                   #
#   See the COPYING file.                                                 #
###########################################################################


BEGIN {

    FS=";"
    KW_SYMBOL_END = -1
}

$1 ~ /^[ \t]*#/ {
    # Mange les commentaires
    $0=""
}

$1 ~ "@END_SYMBOL" {
    KW_SYMBOL_END = KW_ENUM_COUNT
}

$1 ~ /^\+[A-Z]+/ {
    KW_ENUM_COUNT++
}

$1 ~ /^[A-Z]+/ {
    if( KW_SYMBOL_END == -1 )
        print  "\"" $2 "\" { DBG_PRINTF(\"  Symbol " $2 "\\n\"); /*YYLVAL.str = \"" $2 "\";*/ return TOKEN(FRB_KW_TOKEN_" $1 "); }"
        
    KW_ENUM_COUNT++
}

$1 ~  "@ALIAS" {
    if( KW_SYMBOL_END == -1 )
        print  "\"" $3 "\" { DBG_PRINTF(\"  Symbol (alias) " $3 "\\n\"); /*YYLVAL.str = \"" $3 "\";*/ return TOKEN(FRB_KW_TOKEN_" $2 "); }"

}

