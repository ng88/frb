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
    KW_ENUM_COUNT=1024;
    print "%token FRB_KW_TOKEN___BEGIN "
    KW_ENUM_COUNT++
}

$1 ~ /^[ \t]*#/ {
    # Mange les commentaires
    $0=""
}

# $1 ~ /^OP_/ {
#     # Mange les opérateurs
#     $0=""
# }

$1 ~ /^\+[A-Z]+/ {
    KW_ENUM_COUNT++
}


$1 ~ /^[A-Z]+/ {
    #printf "%token FRB_KW_TOKEN_%s  %d\n", $1, KW_ENUM_COUNT
    print "%token FRB_KW_TOKEN_" $1
    KW_ENUM_COUNT++
}

