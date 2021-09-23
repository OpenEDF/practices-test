#!/bin/python
# list

lauguage = 'Python'
lst = list(lauguage)
print(type(lst))
print(lst)

print("-------------------------------")
# [i for i in iterable if expression]
lst = [i for i in lauguage]
print(type(lst))
print(lst)

print("-------------------------------")
add_two_nums = lambda a, b: a + b
print(add_two_nums(2, 3))
