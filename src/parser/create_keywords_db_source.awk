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
    KW_ENUM_COUNT=0
    KW_SYMBOL_END=-1
}

$1 ~ /^[ \t]*#/ {
    # Mange les commentaires
    $0=""
}

$1 ~ "@KW_SET" {

    for(i = 2; i <= NF; ++i)
    {
        kwset[i-2] = $i
    }
    kwsetc=NF - 1
    
    first_set="FRB_SET_" $2
    
    kwsetc=NF - 1
}

$1 ~ "@END_SYMBOL" {
    KW_SYMBOL_END = KW_ENUM_COUNT
}

$1 ~ "+CONSTRUCTOR_NAME" {

    for(i = 0; i < kwsetc; ++i)
        FRB_KEYS[i] = FRB_KEYS[i] "             \""  $(i + 2) "\",\t/* FRB_KW_CONSTRUCTOR_NAME */\n"

    KW_ENUM_COUNT++
}

$1 ~ "+DESTRUCTOR_NAME" {
    for(i = 0; i < kwsetc; ++i)
        FRB_KEYS[i] = FRB_KEYS[i] "             \""  $(i + 2) "\",\t/* FRB_KW_DESTRUCTOR_NAME */\n"

    KW_ENUM_COUNT++
}

$1 ~ /^[A-Z]+/ {
    
    if( KW_SYMBOL_END == -1 )
        FRB_SYMBOLS = FRB_SYMBOLS  "             \"" $2 "\",\t/* FRB_KW_" $1 " */\n"
    else
        for(i = 0; i < kwsetc; ++i)
            FRB_KEYS[i] = FRB_KEYS[i] "             \"" $(i + 2) "\",\t/* FRB_KW_" $1 " */\n"
        
    KW_ENUM_COUNT++
}

END {
    print "#include \"frbkeywords.h\""
    print "#include <cstring>"
    print "#include \"../common/assert.h\"\n"

    print "int FrBKeywords::findKeyword(const char* str) {"
    print "    for(int i = 0; i <  FRB_KW__COUNT - FRB_KW__SYMBOL_END; ++i)"
    print "        if( !strcmp(key[currentSet][i], str) )"
    print "            return i + FRB_KW__SYMBOL_END;\n"
    print "    return FRB_KW__NOTFOUND;\n}\n\n"
    
    print "int FrBKeywords::findKeywordOrSymbol(const char* str) {"
    print "    for(int i = 0; i <  FRB_KW__SYMBOL_END; ++i)"
    print "        if( !strcmp(symbol[i], str) )"
    print "            return i;\n"
    print "    for(int i = 0; i <  FRB_KW__COUNT - FRB_KW__SYMBOL_END; ++i)"
    print "        if( !strcmp(key[currentSet][i], str) )"
    print "            return i + FRB_KW__SYMBOL_END;\n"
    print "    return FRB_KW__NOTFOUND;\n}\n\n"
    
    print "int FrBKeywords::findSymbol(const char* str) {"
    print "    for(int i = 0; i <  FRB_KW__SYMBOL_END; ++i)"
    print "        if( !strcmp(symbol[i], str) )"
    print "            return i;\n"
    print "    return FRB_KW__NOTFOUND;\n}\n\n"

    print "int FrBKeywords::findSet(const char* str) {"
    print "    for(int i = 0; i <  FRB_SET__COUNT; ++i)"
    print "        if( !strcmp(key_set[i], str) )"
    print "            return i;\n"
    print "    return FRB_SET__NOTFOUND;\n}\n\n"
    
    print "const char * FrBKeywords::getKeywordOrSymbol(int index) {"
    print "    frb_assert2(index >= 0 && index < FRB_KW__COUNT, \"frbkeywords.cpp::FrBKeywords::getKeywordOrSymbol(int)\");"
    print "    return (index < FRB_KW__SYMBOL_END) ? symbol[index] : key[currentSet][index - FRB_KW__SYMBOL_END];"
    print "}\n\n"
    
    print "const char * FrBKeywords::getSymbol(int index) {"
    print "    frb_assert2(index >= 0 && index < FRB_KW__SYMBOL_END, \"frbkeywords.cpp::FrBKeywords::getSymbol(int)\");"
    print "    return symbol[index];"
    print "}\n\n"
    
    print "const char * FrBKeywords::getKeyword(int index) {"
    print "    frb_assert2(index >= FRB_KW__SYMBOL_END && index < FRB_KW__COUNT, \"frbkeywords.cpp::FrBKeywords::getKeyword(int)\");"
    print "    return key[currentSet][index - FRB_KW__SYMBOL_END];"
    print "}\n\n"
    
    print "const char* FrBKeywords::getSetName(int index) {"
    print "    frb_assert2(index >= 0 && index < FRB_SET__COUNT, \"frbkeywords.cpp::FrBKeywords::getSetName(int)\");"
    print "    return key_set[index];"
    print "}\n\n"
    
    
    
    print "char* FrBKeywords::key_set[FRB_SET__COUNT] ="
    printf "    {\n"
    
    for(i = 0; i < kwsetc; ++i)
    {
        print "         \"" kwset[i] "\", /* FRB_SET_" kwset[i] " */"
        

        
    }
    
    print "    };\n\n"
    
    print "int FrBKeywords::currentSet = 0; /* default set: " first_set " */\n"
    print "char* FrBKeywords::symbol[FRB_KW__SYMBOL_END] ="
    print "    {"
    print FRB_SYMBOLS
    print "\n    };\n\n"
    
    print "char* FrBKeywords::key[FRB_SET__COUNT][FRB_KW__COUNT - FRB_KW__SYMBOL_END] ="
    print "    {"
    
    for(i = 0; i < kwsetc; ++i)
    {
        printf "        {\n"  FRB_KEYS[i]  "        }"
        
        if( i < (kwsetc - 1) )
            print ", "
    }
    
    print "\n    };\n\n"
}

