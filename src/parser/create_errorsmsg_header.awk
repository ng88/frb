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
    print "#include \"../common/string.h\"\n"
    
    print "/** Class that holds translated error messages */"
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
    print "    static const CString key[FRB_ERR__COUNT];"

    print "    /** Return the message 'index' */"
    print "    inline static const CString getMsg(int index) {"
    print "        frb_assert2(index >= 0 && index < FRB_ERR__COUNT, \"frberrors.h::FrBErrors::getMsg(int)\");"
    print "        return key[index]; }\n"
    
    print "    /** Print the message 'index' on 's' */"
    print "    inline static std::ostream& putMsg(std::ostream& s, int index) {"
    print "        return s << getMsg(index); }\n"
    
    print "    /** Print the message 'index' on 's', replacing '%N' tokens by 'l[N - 1]' */"
    print "    static std::ostream& putMsg(std::ostream&, int index, const StringList& l);"

    print "    /** Print the message 'index' on 's', replacing the '%1' token by 's1' */"
    print "    static std::ostream& putMsg(std::ostream&, int index, const String& s1);"

    print "    /** Print the message 'index' on 's', replacing '%1' & '%2' token by 's1' & 's2' */"
    print "    static std::ostream& putMsg(std::ostream&, int index, const String& s1, const String& s2);"
    
    print "    /** Print the message 'index' on 's', replacing '%1', '%2' & '%3' token by 's1', 's2' & 's3' */"
    print "    static std::ostream& putMsg(std::ostream&, int index, const String& s1, const String& s2, const String& s3);"
    print "    };"
    
    print "#endif\n\n"
}
