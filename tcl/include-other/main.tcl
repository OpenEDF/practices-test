#!/usr/bin/tclsh
### ------------------------------------------------------------------
# 
# Brief: main tcl file invoke the function in other tcl file
# Author: Macro
#

# The first way to inlcude the other file
source test.tcl
printstr

# The second way to inlcude the other file
set test_path [file dirname [info script]]
source [file join $test_path test.tcl]
printstr

# puts the global variable
puts "[lsort [info globals]]"

# puts the global variable from other source file
puts "The global var: $test_var"

### ------------------------------------------------------------------
