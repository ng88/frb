#!/bin/bash

# author: Nicolas GUILLAUME <ng@ngsoft-fr.com>

#
# This script write a piece of the Makefile, it gets the .h dependencies
# of the .cpp/.c files
#

# use: get_dependencies.sh *.cpp

for i in $*
do
    if [ -f $i -a -r $i ]
    then
        # file name without extension
        fname=${i%.*}
        
        # header file name
        hf=$fname.h
        [ -r $hf ] || hf=""
        
        # list of the dependencies
        dep=$(sed -n -e 's/[ ]*#[ ]*include[ ]*"\([^"]*\)".*/\1/gp' $hf $i)
        echo $fname.o: $dep
    fi
done
