#!/bin/bash
count=0
if (test "$#" -eq 1)
then
    while read -r linie;
    do
        rez=$(echo $linie |  grep -E '^[A-Z][a-zA-Z0-9\ \,]*\.$' | grep -E -v 'si[\ ]*\,' | grep -E -v 'n[pb]+' | grep -e "$1")
        if [ -n "$rez" ];
        then
             	count=`expr $count + 1`
        fi
    done
    echo $count
else
    echo "Numar incorect arg"
fi

