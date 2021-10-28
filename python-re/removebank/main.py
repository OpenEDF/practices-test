#!/bin/python
# -*- codind: UTF-8 -*-
import sys

# print the argument and value
print("Arguments numbers: %d, Arguments lists: %s" %(len(sys.argv), sys.argv))

# check the argument number
if (len(sys.argv) < 2):
    print("[ERROR]: Please input the file name.")

# file
file_path = sys.argv[1]

# function
def remove_bank(file_name):
    # open the file 
    linelists = []
    with open(file_name, 'r+') as readfile:
        linelists = readfile.readlines()
    # close file
    readfile.close()

    # remove the bank
    with open(file_name, 'w+') as writefile:
        for index in range(len(linelists)):
            linelists[index] = linelists[index].rstrip()
            print("[Line %d]: Remove bank processing is complete." % index)
            writefile.write(linelists[index] + '\n') 
    # close the file
    writefile.close()

# Run main function
if (__name__ == "__main__"):
    remove_bank(file_path)

# Use manual
# ./main.py test.c
# or: python main.py test.c
