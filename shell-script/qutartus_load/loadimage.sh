#!/bin/bash
ARGS=$1
COMMAND='quartus_pgm -c 1 --mode=JTAG'

POF_CMD="$COMMAND quartus_a10_pof.cdf"
SOF_CMD="$COMMAND quartus_a10_sof.cdf"

# check the pof and sof file
if [[ $ARGS == 'pof' ]]
then
    echo 1111
    $POF_CMD
elif [[ $ARGS == 'sof' ]] 
then
    echo 222222222
    $SOF_CMD
else
    echo 33333333
fi
