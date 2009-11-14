#!/bin/bash

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
#   See the COPYING file.                                                 #
###########################################################################



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



