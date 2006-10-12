#!/bin/bash

# Author: ng <ng@ngsoft-fr.com>

#Parameters : add_header header files

if [ $# -lt 2 ]
then
    echo "usage: ${0##*/} header_file source_file1 ... source_filen" >&2
    exit 1
fi

$tmp=/tmp/add_header__$$_f.tmp
$header="$1"

shift

for i in $*
do
    echo $i...
    cp -f "$i" "$tmp"
    cat "$header" > "$i"
    cat "$tmp"   >> "$i"
done

echo Done.

rm -f $tmp

