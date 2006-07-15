#! /bin/bash

#author : ng <ng@ngsoft-fr.com>

if [ $# -ne 4 ]
then
    echo "Usage: ${0##*/} label destination source1 source2" >&2
    echo "Copy 'source1' into 'destination', if 'label' is a line of source1 then 'label' is replaced by the content of 'source2'" >&2
    exit 1
fi

exec 0< "$3"
exec  > "$2"

IFS=

while read -r i
do
    if [ "$i" = "$1" ]
    then
        cat "$4"
    else
        echo "$i"
    fi
done
