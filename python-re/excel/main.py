#!/usr/bin/python3.8
#--------------------------------------------------------------------
# File:   main.py
# Author: Macro
# Date:   2022-05-11
# Breif:  Sales data analysis for XiangHe
# TODO:
#      1. reading and  wirtting the data
#      2. pivot table
#      3. 
#--------------------------------------------------------------------
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

sale_file = "sale_sample.xls"

# Read the xls file
df = pd.read_excel(sale_file)
print(df, "\n")

# pivot the Region and Item
pd_region = pd.pivot_table(df, index=['Region', 'Rep'], columns='Item', values = 'Total', aggfunc = 'sum', margins = True, fill_value=0)
print(pd_region, "\n")

# piovt the Item and Rep
pd_item = pd.pivot_table(df, index='Item', columns='Rep', values = 'Total', aggfunc = 'sum', fill_value=0)
print(pd_item, "\n")

# Multiply each number in the table by 10
for i in range(5):
    for j in range(11):
        temp = pd_item.iat[i, j] * 10
        pd_item.iat[i, j] = round(temp, 2)

print(pd_item, "\n")

# Plot the chart and save to .png
pd_item.plot(kind='bar')
plt.title("Sale data analysis")
plt.xlabel("Rep")
plt.ylabel("Item")
plt.savefig('test.png')
plt.show()

# Create a dataframe into a list of rows
list_row = pd_item.to_numpy().tolist()
print(list_row)
print(list_row[0])

# Create a dataframe into a list of column
list_colunm = pd_item.transpose().values.tolist()
print(list_colunm)
print(list_colunm[0])

# Get the excel information by pandas
list_columns_value = pd_item.columns.values
print(list_columns_value)
print(pd_item.columns)
print(pd_item.keys())

# Sort the excel
pd_sort = pd_item.sort_values(by = 'Jones', ascending=False, kind='mergesort')
print(pd_sort)

# dataframe append
pd_append = pd.DataFrame({"XiangHe":[11.11, 22.22, 33.33, 44.44, 55.55]}, index=[0, 1, 2, 3, 4])
print(pd_append)
pd_append_new = pd_sort.append(pd_append, sort=True, ignore_index=False)
print(pd_append_new)
#----------------------------- END ----------------------------------
