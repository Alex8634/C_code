#!/bin/bash

dir=$1
file=$2

count=0
countRec=0
if [ $# -ne 2 ]
then
    echo "nu-i ok"
fi

for entry in "$dir"/*
do
    if [ -L $entry ]
    then
	count=$((count++))
    elif [ -f $entry ]
    then
       rez = cat $entry |  grep -E '^[A-Z][a-zA-Z0-9\ \,]*\.$' | grep -E -v 'si[\ ]*\,' | grep -E -v 'n[pb]+'
	
       if [ ! -z $rez ]
       then
	   echo $entry >> $file
       fi 

    
   elif [ -d $entry ]
    then
	countRec= 'bash $0 $entry $file'
	count=$((count + $countRec))
    fi
    
done


