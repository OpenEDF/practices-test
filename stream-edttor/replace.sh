#!/bin/sh
echo --------------------------SED---------------------------------
echo original file
echo 
echo unix is great os. unix is opensource. unix is free os.
echo learn operating system.
echo unix linux which one you choose.
echo unix is easy to learn.unix is a multiuser os.Learn unix .unix is a powerful.
echo
echo --------------------------------------------------------------
(set -x; sed 's/unix/linux/'   lines.txt)
echo --------------------------------------------------------------
(set -x; sed 's/unix/linux/1'  lines.txt)
echo --------------------------------------------------------------
(set -x; sed 's/unix/linux/2'  lines.txt)
echo --------------------------------------------------------------
(set -x; sed 's/unix/linux/g'  lines.txt)
echo --------------------------------------------------------------
(set -x; sed 's/unix/linux/3g' lines.txt)

