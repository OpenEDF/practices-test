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
#----------------------------- END ----------------------------------
