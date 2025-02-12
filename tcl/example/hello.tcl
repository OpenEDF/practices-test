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

# list
set colorlist1 {red green yellow}
set colorlist2 [list red green blue]
set colorlist3 [split "red_green_blue" _]
puts $colorlist1
puts $colorlist2
puts $colorlist3
append colorlist1 " " "blue"
lappend colorlist1 "orange"
puts $colorlist1
puts [llength $colorlist1]

# dictionary
set colours [dict create coclour1 "black" colour2 "white"]
foreach item [dict keys $colours] {
    set value [dict get $colours $item]
    puts $value
}
set ret [dict exists $colours coclour1]
puts $ret

# procdedures
proc helloworld {} {
    puts "Hello World!"
}

proc add {a b} {
    return [expr $a + $b]
}

proc add_1 {a {b 100}} {
    return [expr $a + $b]
}

proc add_2 {a {b 100}} {
set c 10
    if {$a < 10} {
        return [expr 10 + $c]
    }
    return [expr $a + $b]
}

# main
helloworld
puts [add 10 30]
set c 20
puts [add_1 9]
puts "c:$c"
