#!/bin/python

def generate_regmap(reg_file_path, map_file_path):
    linelists = []
    with open(reg_file_path, 'r') as f:
        linelists = f.readlines()
    f.close()

    # remove #define
    for index in range(len(linelists)):
        if ("#define" in linelists[index]):
            linelists[index] = linelists[index][8:]
        else:
            linelists[index] = ""

    # write to new file
    with open(map_file_path, 'w') as output:
        for line in linelists:
            output.write(line)
    output.close()
