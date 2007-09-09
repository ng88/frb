#!/bin/bash

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
#   (at your option) any later version.                                   #
#   See the COPYING file.                                                 #
###########################################################################

# Execute all the test in the folder passed as argument
# for each .frb file in the directory, a .frb.result file
# must exists (containing the expected result)



# USAGE
# type ./test_folder noregression to execute the no regression tests
#
# to create your own test, juste put a .frb file in a directory and run
# the following commands :
#
#    $FRB_PATH --file YOUR_FILE.frb > YOUR_FILE.frb.result 2>&1
#    sed -i -e 's/0x[0-9a-fA-F][0-9a-fA-F]*/@ADDR@/g YOUR_FILE.frb.result
#


[ "$FRB_PATH" = "" ] && FRB_PATH=../parser/frbc

if ! [ -x "$FRB_PATH" ]
then
    echo "unable to execute FrB ($FRB_PATH), please set the FRB_PATH environment variable." >&2
    exit 1
fi

if [ "$1" = "" -o ! -d "$1" ]
then
    echo usage: $0 folder >&2
    exit 2
fi


cd $1

echo '############' $1 '############'

for i in *.frb
do
    if [ -r $i -a -r $i.result ]
    then
	echo -n "Test $i: "
	cd - > /dev/null
	"$FRB_PATH" --file $1/$i 2>&1 | sed -e 's/0x[0-9a-fA-F][0-9a-fA-F]*/@ADDR@/g' | diff -u - $1/$i.result
	if [ $? -eq 0 ]
	then
	    echo success
	else
	    echo FAILED
	    exit 3
	fi
	cd - > /dev/null
    else
        echo "warning: can't read $1/$i or $1/$i.result" >&2
    fi
done

echo Done.
