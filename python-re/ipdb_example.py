#!/bin/python
import ipdb

# debug
ipdb.set_trace()

# python loop
def test_loop(count):
    while count < 5:
        print("count less than 5: ", count)
        count = count + 1
    else:
        print("The current conut result is: ", count)
    return None

# python for
lists = ['red', 'green', 'blue', 'yellow', 'orange', 'purple', 'balck', 'white']
def test_for():
    for color in lists:
        print("The color is: ", color)
    else:
        print("all color list is showed.")
    return None

# main
def main():
    test_loop(2)
    print("----------------------------------------------------------------------")
    test_for()

    return 0

# call and invoke
if __name__ == "__main__":
    main()
