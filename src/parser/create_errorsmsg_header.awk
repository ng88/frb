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

    print "#ifndef FRBERRORS_H"
    print "#define FRBERRORS_H"
    print "#include <sstream>"
    print "#include \"../common/assert.h\"\n"
    
    print "class FrBErrors {\npublic:"
    print "    enum { "
    print "        FRB_ERR__NOTFOUND = -1,"
    
    FS=";"
    ERR_ENUM_COUNT=0
    
}

$1 ~ /^[ \t]*#/ {
    # Mange les commentaires
    $0=""
}

$1 ~ /^[A-Z]+/ {
    printf "        FRB_ERR_%s = %d,\n", $1, ERR_ENUM_COUNT


    ERR_ENUM_COUNT++
}

END {
    print "        FRB_ERR__COUNT };\n"
    print "    enum { FRB_ERR_DISPLAYSHIFT = 10 };"
    print "    static char* key[FRB_ERR__COUNT];"
    print "    inline static char* getMsg(int index) {"
    print "        frb_assert2(index >= 0 && index < FRB_ERR__COUNT, \"frberrors.h::FrBErrors::getMsg(int)\");"
    print "        return key[index]; }"
    print "    };"
    
    print "#endif\n\n"
}
