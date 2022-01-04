import os

array = []
cwd = os.getcwd()
array.append(cwd)

os.chdir('../')
array.append(os.getcwd())
print("Current working directory:", array)
