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


BEGIN {

    print "#include \"frberrors.h\""
    
        
    print "std::ostream& FrBErrors::putMsg(std::ostream& o, int index, const StringList& l)"
    print "{"
    print "    frb_assert2(index >= 0 && index < FRB_ERR__COUNT, \"frberrors.h::FrBErrors::putMsg()\");"
    print ""
    print "    int i = 0;"
    print "    const CString str = key[index];"
    print "    char c;"
    print "    while( (c = str[i]) )"
    print "    {"
    print "        if(c == '%' && str[i + 1])"
    print "        {"
    print "            int v = str[i + 1] - '0';"
    print "            if( v > 0 && v <=9 && v <= (int)l.size())"
    print "            {"
    print "                o << l[v - 1];"
    print "                i++;"
    print "            }"
    print "            else"
    print "                o << '%';"
    print ""
    print "        }"
    print "        else"
    print "            o << c;"
    print ""
    print "        i++;"
    print "    }"
    print ""
    print "    return o;"
    print "}\n\n"
    
    
    print "std::ostream& FrBErrors::putMsg(std::ostream& o, int index, const String& s1)"
    print "{"
    print "    StringList l;"
    print "    l.push_back(s1);"
    print "    return putMsg(o, index, l);"
    print "}\n\n"
    
    print "std::ostream& FrBErrors::putMsg(std::ostream& o, int index, const String& s1, const String& s2)"
    print "{"
    print "    StringList l;"
    print "    l.push_back(s1);"
    print "    l.push_back(s2);"
    print "    return putMsg(o, index, l);"
    print "}\n\n"
    
    print "std::ostream& FrBErrors::putMsg(std::ostream& o, int index, const String& s1, const String& s2, const String& s3)"
    print "{"
    print "    StringList l;"
    print "    l.push_back(s1);"
    print "    l.push_back(s2);"
    print "    l.push_back(s3);"
    print "    return putMsg(o, index, l);"
    print "}\n\n"
    
    print "const CString FrBErrors::key[FRB_ERR__COUNT] = {"
    
    FS=";"
    
}

$1 ~ /^[ \t]*#/ {
    # Mange les commentaires
    $0=""
}

$1 ~ /^[A-Z]+/ {
    print "     /* FRB_ERR_" $1 " */"
    print  "    " $2 ","
    

}

END {
    print "    };"
}