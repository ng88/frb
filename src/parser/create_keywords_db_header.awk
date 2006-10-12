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

    print "#ifndef FRBKEYWORDS_H"
    print "#define FRBKEYWORDS_H"
    
    print "class FrBKeywords {\npublic:"
    print "    enum { "
    print "        FRB_KW__NOTFOUND = -1,"
    
    FS=";"
    KW_ENUM_COUNT=0
    KW_SYMBOL_END=0
    
}

$1 ~ /^[ \t]*#/ {
    # Mange les commentaires
    $0=""
}



$1 ~ "@END_SYMBOL" {
    KW_SYMBOL_END = KW_ENUM_COUNT
}

$1 ~ "@KW_SET" {
    for(i = 2; i <= NF; ++i)
    {
        kwset[i-2] = $i
    }
    kwsetc=NF - 1
}

$1 ~ "+CONSTRUCTOR_NAME" {
    printf "        FRB_KW_CONSTRUCTOR_NAME = %d,\n", KW_ENUM_COUNT
    
    KW_ENUM_COUNT++
}

$1 ~ "+DESTRUCTOR_NAME" {
    printf "        FRB_KW_DESTRUCTOR_NAME = %d,\n", KW_ENUM_COUNT
    
    KW_ENUM_COUNT++
}

$1 ~ /^[A-Z]+/ {
    printf "        FRB_KW_%s = %d,\n", $1, KW_ENUM_COUNT


    KW_ENUM_COUNT++
}

END {
    print "        FRB_KW__COUNT };\n    enum frb_set_t {\n        FRB_SET__NOTFOUND = -1,"
    for(i = 0; i < kwsetc; ++i)
    {
        printf "        FRB_SET_%s = %d,\n", kwset[i], i
    }
    print "        FRB_SET__COUNT };"
    print "    enum { FRB_KW__SYMBOL_END = " KW_SYMBOL_END " };"
    print "    static int findKeyword(const char* str); /* find if str is a keyword [A-Z][a-z]* */"
    print "    static int findKeywordOrSymbol(const char* str); /* find if str is a keyword [A-Z][a-z] or a symbol */"
    print "    static int findSymbol(const char* str); /* find if str is a symbol (ie +, *, -, ...) */"
    print "    static const char* getKeyword(int index); /* return the keyword for corresponding index ( FRB_KW__SYMBOL_END <= index < FRB_KW__COUNT ) */"
    print "    static const char* getSymbol(int index); /* return the symbol for corresponding index ( 0 <= index < FRB_KW__SYMBOL_END ) */"
    print "    static const char* getKeywordOrSymbol(int index); /* return the keyword for corresponding index ( 0 <= index < FRB_KW__COUNT ) */\n"
    print "    static int findSet(const char* str); /* find set id for corresponding set name */"
    print "    static const char* getSetName(int index); /* return the set name for corresponding index ( 0 <= index < FRB_SET__COUNT ) */\n"
    print "    inline static int getCurrentSet() { return currentSet; }"
    print "    inline static void setCurrentSet(int i) { currentSet = i; }"
    print "private:"
    print "    static int currentSet;"
    print "    static char* key[FRB_SET__COUNT][FRB_KW__COUNT - FRB_KW__SYMBOL_END]; /* keyword list */"
    print "    static char* symbol[FRB_KW__SYMBOL_END]; /* symbol list */"
    print "    static char* key_set[FRB_SET__COUNT]; /* keyword set list */\n\n };\n"
    
    print "#endif\n\n"
}