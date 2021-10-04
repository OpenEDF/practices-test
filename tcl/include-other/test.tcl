#!/usr/bin/tclsh
### ------------------------------------------------------------------
# 
# Brief: the proc will be call for other file
# Author: Macro
#

# the test proc for put string
proc printstr {} {
    puts "The test proc id from test.tcl file."
}

# the test variable
set test_var 1
puts "The test_var variable is in test.tcl: $test_var"
### ------------------------------------------------------------------
