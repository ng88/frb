#!/bin/bash

#
# Affiche la version de 'prog'
#   getVersion prog [version_param]
#   version_param est le paramètre permettant de
#   récupérer la version, vaut --version par défaut
#

function getVersion()
{
    if [ "$2" = "" ]
    then
        vparam="--version"
    else
        vparam="$2"
    fi
    
    echo -n "$1 version: "
    if ! which $1 > /dev/null 2>&1
    then
        echo "program not found!"
        return 1;
    fi
    
    ($1 $vparam 2> /dev/null) | awk -F'[,; \t()]' 'NR == 1 { for(i=1; i <= NF; ++i) { if( $i ~ /[0-9]+\.[0-9]+/ ) { print $i; break} } }'

    
    return 0;
}

getVersion "g++"
getVersion "flex++"
getVersion "bison++"
getVersion "awk"



