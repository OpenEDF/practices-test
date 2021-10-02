#!/usr/bin/tclsh
# 
# proc procedureName {arguments} {
#   body
# }
#

set glo 10

proc main {} {
    puts "Hello World!"
}

proc add {a b} {
    return [expr $a + $b]
}

proc avg {number} {
    set sum [expr $number + 10]
    main
    return $sum
}

# run main
main
puts [add 10 30]
puts [avg $glo]
