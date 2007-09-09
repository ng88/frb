#!/bin/bash

# author: Nicolas GUILLAUME <ng@ngsoft-fr.com>

#
# This script write a piece of the Makefile, it gets the .h dependecies
# of the .cpp/.c files
#

# use: get_dependencies.sh *.cpp

for i in $*
do
    hf=$(sed -n -e 's/[ ]*#[ ]*include[ ]*"\([^"]*\)".*/\1/gp' $i)
    cf=${i%.*}
    echo $cf.o: $hf
done
