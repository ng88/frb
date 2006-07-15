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
        print  "\"" $2 "\" { DBG_PRINTF(\"  Symbol " $2 "\\n\"); YYLVAL.str = \"" $2 "\"; return TOKEN(FRB_KW_TOKEN_" $1 "); }"
        
    KW_ENUM_COUNT++
}

$1 ~  "@ALIAS" {
    if( KW_SYMBOL_END == -1 )
        print  "\"" $3 "\" { DBG_PRINTF(\"  Symbol (alias) " $3 "\\n\"); YYLVAL.str = \"" $3 "\"; return TOKEN(FRB_KW_TOKEN_" $2 "); }"

}

