#! /bin/bash

###########################################################################
#   This file is part of the FrB project                                  #
#                                                                         #
#   FrB project                                                           #
#                                                                         #
#   Copyright (C) 2006, 2007 by GUILLAUME Nicolas                         #
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
# Compare la version des outils nécessaires avec ceux
# de la machine courante
#

TMP=${TMPDIR:-/tmp}/check_needed.sh.$$
trap "rm $TMP* 2>/dev/null" EXIT

./build_needed.sh > $TMP

if diff -bBi NEEDED $TMP
then
    echo "Versions ok!"
    exit 0
else
    echo -e "\n\nDifferent versions, see above."
    exit 1
fi
