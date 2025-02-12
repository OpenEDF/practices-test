#!/usr/bin/tclsh
puts "Hello World!" ;# comments
puts $tcl_version
puts $env(PATH)
puts $argc
puts $argv
# print the file name
puts $argv0

puts $tcl_rcFileName

# other multiine or block comment
if 0 {
    multiline
    or block comment
    test content
}

# print sum
puts [expr 100 + 198]

# variable
set macro 19
puts $macro
puts [expr $macro + 20]

# backslash
puts "This is a \nbackslash test"

# string
set str "shanghai and New York"
set str1 test
set str2 {shanghai and New York}
puts $str
puts $str1
puts $str2
puts [lindex $str2 0]
puts [lindex $str2 2]

# handles
set myfiles [open "test.txt" w+]
puts $myfiles "test"
close $myfiles
set fp [open "test.txt" r]
set file_data [read $fp]
puts $file_data
close $fp

# matchematical
set tcl_precision 5
set result [expr $macro / 9.0]
puts $result

# decisions
set a 10
set b [expr $a == 1 ? 20 : 50]
puts "value of b is $b \n"

# loop
set index 10
while { $index } {
    puts "this loop will forever: $index"
    incr index -1
}
