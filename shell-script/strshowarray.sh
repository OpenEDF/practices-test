#!/bin/bash
ORIGINAL_DATA=$1

# show the original data
echo $ORIGINAL_DATA

# calculater the length for data
STRLENGTH=${#ORIGINAL_DATA}
echo "The string length: $STRLENGTH".

# show string to array
for (( index=0; index<=$STRLENGTH; index++))
do
    echo "${ORIGINAL_DATA[$index]}" 
done    
