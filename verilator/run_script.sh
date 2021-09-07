#!/bin/bash
echo "################ RISC-V CPU  ###################"
if [ -d "obj_dir" ]; then
    rm -r obj_dir
fi

# To create the obj_dir
verilator -Wall --sc our.v --exe sc_main.cpp

# To compiler
make -j -C obj_dir -f Vour.mk Vour
