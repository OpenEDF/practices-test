1. test command:
splint main.c

2. splint output result
Splint 3.1.2 --- 20 Feb 2018

main.c: (in function main)
main.c:7:17: Operands of < have incompatible types (int, long int): i < 10l
  To ignore type qualifiers in type comparisons use +ignorequals.
main.c:12:2: Path with no return in function declared to return int
  There is a path through a function declared to return a value on which there
  is no return statement. This means the execution may fall through without
  returning a meaningful result to the caller. (Use -noret to inhibit warning)

Finished checking --- 2 code warnings
