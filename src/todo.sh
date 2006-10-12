#!/bin/bash

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


nb_todo=0
nb_tot_todo=0
nb_todo_fichier=0
nb_fichier=0

for i in $(find -name '*.h' -o -name '*.cpp' -o -name '*.awk' -o -name '*.y' -o -name '*.l')
do
      ((nb_fichier++))
      nb_todo=$(grep -c  '//TODO' $i)
      if [ $? -eq 0 ]
      then
          ((nb_todo_fichier++))
          ((nb_tot_todo+=nb_todo))
          echo "$i ($nb_todo): "
          grep -n  '//TODO' $i | sed 's/\([0-9]\+\).*\/\/TODO/    *> l\1: /g'
      fi
done

echo '**' "$nb_tot_todo TODOs in $nb_todo_fichier files ($nb_fichier files examinated)" '**'

