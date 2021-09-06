#!/bin/bash
ARGS=$1
LOAD_COMMAND='quartus_pgm -c 1 --mode=JTAG'
CHECK_USB=$(lsusb | grep "Altera Blaster")

POF_CMD="$LOAD_COMMAND quartus_a10_pof.cdf"
SOF_CMD="$LOAD_COMMAND quartus_a10_sof.cdf"

# check the usb connect
if [[ $CHECK_USB == "" ]]
then
    echo "[ERROR] Please connect the development board to your computer and check usb driver."
    exit
else
    echo "[OK] An available USB device was found: $CHECK_USB."
fi

# check the pof and sof file and progarmming
if [[ $ARGS == 'pof' ]]
then
    if [[ -e hawk_top.pof ]]
    then
        echo "[OK] Find the .pof file."
        echo "load image file to flash........"
        $POF_CMD
    else
        echo "[ERROR] No available .pof files were found."
        exit
    fi
elif [[ $ARGS == 'sof' ]] 
then
    if [[ -e hawk_top.sof ]]
    then
        echo "[OK] Find the .sof file."
        echo "load image sof file to ram........"
        $SOF_CMD
    else
        echo "[ERROR] No available .sof files were found."
        exit
    fi
else
    echo [INFO] "Please enter the correct command format."
    echo [INFO] $ "./loadimage.sh sof/pof"
    exit
fi

# finish
echo [INFO] "Operation Finish."

# ############################## END ##################################
