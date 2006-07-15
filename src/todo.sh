#!/bin/bash


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

