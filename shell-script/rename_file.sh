#!/bin/bash
files=$(find -name Makefile.vcs)
echo $files

for file in $files; do
    echo "Processing $file"
    new_file=${file%.*} 
    echo $new_file
    mv -v $file $new_file
done

# rename all Makefile.vs to Makefile
