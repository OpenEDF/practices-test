#!/usr/bin/tclsh
# 
# proc procedureName {arguments} {
#   body
# }
#

set glo 10
set ::c 2   ;# global veriable
set d 30

proc main {} {
    puts "Hello World!"
}

proc add {a b} {
    return [expr $a + $b]
}

proc avg {number} {
    set sum [expr $number + $::c]
    main
    return $sum
}

proc gloarg {} {
    global d
    set d [expr 10 + $d]
    return $d
}

# run main
main
puts [add 10 30]
puts [avg 10]
puts $c
puts $d
puts [ gloarg ]
puts $d
