#!/bin/bash
declare -a array=("add" 
                  "sub"
                  "sii")

for index in "${array[@]}";
do
    echo "$index"
    make clean
    make run case=$index
done
