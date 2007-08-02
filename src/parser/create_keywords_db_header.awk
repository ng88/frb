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
    print "#include \"frbconstants.h\""
    print "#include \"../common/string.h\"\n"

    print "/** Class that holds keyword of the different set"
    print "     Note: CurrentSet is static for convenience. A better solution may be used in future\n*/"
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

    print "    /** Find if str is a keyword (ie [A-Z][a-z]*) */"
    print "    static int findKeyword(const CString str);"

    print "    /** Find if str is a keyword (ie [A-Z][a-z]*) or a symbol (ie +, *, -, ...) */"
    print "    static int findKeywordOrSymbol(const CString str);"

    print "    /** Find if str is a symbol (ie +, *, -, ...) */"
    print "    static int findSymbol(const CString str);"

    print "    /** Return the keyword for corresponding index (FRB_KW__SYMBOL_END <= index < FRB_KW__COUNT) */"
    print "    static const CString getKeyword(int index);"

    print "    /** Return the symbol for corresponding index (0 <= index < FRB_KW__SYMBOL_END) */"
    print "    static const CString getSymbol(int index);"

    print "    /** Return the keyword for corresponding index (0 <= index < FRB_KW__COUNT) */"
    print "    static const CString getKeywordOrSymbol(int index);\n"

    print "    /** Find set id for corresponding set name */"
    print "    static int findSet(const CString str);"

    print "    /** Return the set name for corresponding index ( 0 <= index < FRB_SET__COUNT ) */"
    print "    static const CString getSetName(int index);\n"

    print "    inline static int getCurrentSet() { return currentSet; }"
    print "    inline static void setCurrentSet(int i) { currentSet = i; }\n"

    print "    /** Return FRB_KW_BYVAL string if vb is true, FRB_KW_BYREF string otherwise */"
    print "    inline static const CString byvalToString(bool vb)"
    print "       { return getKeyword(vb ? FRB_KW_BYVAL : FRB_KW_BYREF); }"

    print "    /** Return FRB_KW_SHARED string if shared is true, \"\" string otherwise */"
    print "    inline static const CString sharedToString(bool shared)"
    print "       { return shared ? getKeyword(FRB_KW_SHARED) : \"\"; }"

    print "    /** Return FRB_KW_CONST string if cst is true, \"\" string otherwise */"
    print "    inline static const CString constToString(bool cst)"
    print "       { return cst ? getKeyword(FRB_KW_CONST) : \"\"; }"

    print "    /** Return FRB_KW_ABSTRACT string if abs is true, \"\" string otherwise */"
    print "    inline static const CString abstractToString(bool abs)"
    print "       { return abs ? getKeyword(FRB_KW_ABSTRACT) : \"\"; }"

    print "    /** Return FRB_KW_SEALED string if s is true, \"\" string otherwise */"
    print "    inline static const CString sealedToString(bool s)"
    print "       { return s ? getKeyword(FRB_KW_SEALED) : \"\"; }"

    print "    /** Return the string corresponding to the scope s (ie FRB_KW_PUBLIC or FRB_KW_PRIVATE string ...) */"
    print "    static const CString scopeToString(int s);"

    print "\nprivate:"
    print "    static int currentSet;"
    print "    static CString key[FRB_SET__COUNT][FRB_KW__COUNT - FRB_KW__SYMBOL_END]; /* keyword list */"
    print "    static CString symbol[FRB_KW__SYMBOL_END]; /* symbol list */"
    print "    static CString key_set[FRB_SET__COUNT]; /* keyword set list */\n\n };\n"
    
    print "#endif\n\n"
}
