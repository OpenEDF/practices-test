#!/usr/bin/tclsh
puts "Hello World!"
puts $tcl_version

# the special argument
puts $tcl_version

# the number of cli arguments
puts "The number of command line command: $argc"
puts "The first elements: [lindex $argv 0]"
puts "The second elements: [lindex $argv 1]"

# print the filename
puts "The file name is: $argv0"

# print the list for arguments
puts "The arguments list: $argv"

# print the tcl platform
puts "The current platform: [array size tcl_platform]"
