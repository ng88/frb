###########################################################################
#   This file is part of the FrB project                                  #
#                                                                         #
#   FrB project                                                           #
#                                                                         #
#   Copyright (C) 2006 - 2010 by GUILLAUME Nicolas                        #
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


# generate wiki keyword table

BEGIN {
    FS=";"
    KW_ENUM_COUNT=0
    KW_SYMBOL_END=-1
}

$1 ~ /^[ \t]*#/ {
    # Mange les commentaires
    $0=""
}

$1 ~ "@KW_SET" {

    printf "||Keyword name||"
    for(i = 2; i <= NF; ++i)
    {
        printf $i"||"

    }
    kwsetc = i - 2
    print ""

}

$1 ~ "@END_SYMBOL" {
    KW_SYMBOL_END = KW_ENUM_COUNT
}

$1 ~ "+CONSTRUCTOR_NAME" {

    printf "||FRB_KW_CONSTRUCTOR_NAME||"
    for(i = 0; i < kwsetc; ++i)
        printf $(i + 2)"||"

    print ""

    KW_ENUM_COUNT++
}

$1 ~ "+DESTRUCTOR_NAME" {

    printf "||FRB_KW_DESTRUCTOR_NAME||"
    for(i = 0; i < kwsetc; ++i)
        printf $(i + 2)"||"

    print ""

    KW_ENUM_COUNT++
}

$1 ~ /^[A-Z]+/ {
    
    if( KW_SYMBOL_END == -1 )
    {

    }
    else
    {
       printf "||FRB_KW_"$1"||"
       for(i = 0; i < kwsetc; ++i)
       {

          # if the keyword can be a wiki page name, we must escape it
          if( $(i + 2) ~ /^[A-Z]+[a-z]+[A-Z]+[a-z]+/ )
             printf "!"

           printf $(i + 2)"||"
       }
       print ""
    }
        
    KW_ENUM_COUNT++
}



