#!/bin/python3
## test code for python tools 2to3
## 
def greet(name):
    print("Hello, {0}!".format(name))

print("What's your name?")

name = input()
greet(name)
