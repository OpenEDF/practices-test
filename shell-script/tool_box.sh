#!/bin/bash

# 1. change the file extension
for file in path/*.asm; do mv -v -- "$file" "${file%.asm}.S";done
