#!/usr/bin/env python
# To set and get environment varibale in python use os module

import os
import subprocess

# set environment variables
os.environ['TEST_ENV_USER'] = "macro"
os.environ['TEST_ENV_AGE'] = "27"

# get environment variables
user = os.getenv('TEST_ENV_USER')
age = os.getenv('TEST_ENV_AGE')

# getting non-existent keys
FOO = os.getenv('FOO')
BAR = os.environ.get('BAR')
#BAZ =os.environ['BAZ'] #KeyError: 'BAZ'

# print
print(user)
print(age)
print(FOO)
print(BAR)

# get cmd output
output = subprocess.getoutput("pwd")
print(output)

