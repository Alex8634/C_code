#!/bin/bash


while read linie
do

    echo $linie |  grep -E '^[0-255]\.[0-255]\.[0-255]\.[0-255]$'
done
