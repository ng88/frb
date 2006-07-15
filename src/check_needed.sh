#! /bin/bash

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
