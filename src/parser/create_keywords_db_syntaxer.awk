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

