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
        FRB_KEYS[i] = FRB_KEYS[i] "             <item> "  $(i + 2) " </item>\n"

    KW_ENUM_COUNT++
}

$1 ~ "+DESTRUCTOR_NAME" {
    for(i = 0; i < kwsetc; ++i)
        FRB_KEYS[i] = FRB_KEYS[i] "             <item> "  $(i + 2) " </item>\n"

    KW_ENUM_COUNT++
}

$1 ~ /^[A-Z]+/ {
    
    if( KW_SYMBOL_END == -1 )
        FRB_SYMBOLS = FRB_SYMBOLS  "             <item> " $2 " </item>\n"
    else
        for(i = 0; i < kwsetc; ++i)
            FRB_KEYS[i] = FRB_KEYS[i] "             <item> " $(i + 2) " </item>\n"
        
    KW_ENUM_COUNT++
}

END {
    print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    print "<!DOCTYPE language SYSTEM \"language.dtd\">"
    print "<language name=\"FrB\" version=\"1.00\" kateversion=\"2.1\" section=\"Sources\" extensions=\"*.frb\" mimetype=\"text/x-frb-src\" license=\"GPL\" author=\"Nicolas GUILLAUME &lt;ng@ngsoft-fr.com&gt;\">"
    print "    <highlighting>"
    print "        <list name=\"keywords\">"
    
    
    for(i = 0; i < kwsetc; ++i)
    {
        printf FRB_KEYS[i]
    }
    
    print "            <item> DefLang </item>"
    print "        </list>"
    print "        <list name=\"types\">"
    print "            <item>Integer </item>"
    print "            <item>Boolean </item>"
    print "            <item>Bool </item>"
    print "            <item>Int </item>"
    print "            <item>String </item>"
    print "        </list>"
    print "        <contexts>"
    print "            <context attribute=\"Normal Text\" lineEndContext=\"#stay\" name=\"Normal\">"
    print "                <keyword attribute=\"Keyword\" context=\"#stay\" String=\"keywords\"/>"
    print "                <keyword attribute=\"Identifier\" context=\"#stay\" String=\"types\"/>"
    print "                <Float attribute=\"String\" context=\"#stay\"/>"
    print "                <Int attribute=\"Types\" context=\"#stay\"/>"
    print "                <DetectChar attribute=\"String\" context=\"String\" char=\"&quot;\"/>"
    print "                <DetectChar attribute=\"Comment\" context=\"Comment\" char=\"'\"/>"
    print "        </context>"
    print "        <context name=\"Comment\" attribute=\"Comment\" lineEndContext=\"#pop\"/>"
    print "        <context name=\"String\" attribute=\"String\" lineEndContext=\"#pop\">"
    print "            <DetectChar attribute=\"String\" context=\"#pop\" char=\"&quot;\"/>"
    print "        </context>"
    print "        </contexts>"
    print "        <itemDatas>"
    print "            <itemData name=\"Normal Text\" defStyleNum=\"dsNormal\"/>"
    print "            <itemData name=\"Keyword\" defStyleNum=\"dsKeyword\"/>"
    print "            <itemData name=\"Identifier\" defStyleNum=\"dsOthers\"/>"
    print "            <itemData name=\"Types\" defStyleNum=\"dsDataType\"/>"
    print "            <itemData name=\"String\" defStyleNum=\"dsString\"/>"
    print "            <itemData name=\"Comment\" defStyleNum=\"dsComment\"/>"
    print "        </itemDatas>"
    print "          </highlighting>"
    print "          <general>"
    print "            <comments>"
    print "              <comment name=\"singleLine\" start=\"'\" />"
    print "            </comments>"
    print "            <keywords casesensitive=\"1\" />"
    print "          </general>"
    print "        </language>"
    

}

