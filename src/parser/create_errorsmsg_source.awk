BEGIN {

    print "#include \"frberrors.h\""
    
    print "char* FrBErrors::key[FRB_ERR__COUNT] = {"
    
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