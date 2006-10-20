#!/bin/bash

i=1
while svn log -r$i $* 2>/dev/null
do
    (( i++ ))
done

echo revision: $(( i-1 ))
exit 0