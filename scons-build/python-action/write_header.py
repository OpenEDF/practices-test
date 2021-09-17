#!/usr/bin/python
import time

def scons_py_build(taget, source, env):
    # 1. read the time
    ticks = time.time()
    localtime = time.asctime(time.localtime(ticks))
    print( "Local current time:", localtime)
    localtime = "/* Time & Date: " + localtime + " */ \n"

    lines = []
    lines.append(localtime)
    string = "/* Hello World! */\n"
    print(lines)

    # 2. open the file
    file1 = open("myfile.h", "a")

    # 3. write file
    file1.write(string)
    file1.writelines(lines)

    # 4. close file
    file1.close()
    
    return None

def main():
    scons_py_build()

if __name__ == "__main__":
    mian()

## end



