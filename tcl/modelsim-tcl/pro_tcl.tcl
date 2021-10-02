###-----------------------------------------------------------------
### modelsim tcl file
### Author: Macro
### Resources: ModelSim® SE User's Manual Software Version 2019.4
### Chapter 31 Tcl and DO Files

### --1. print the help information
puts " "
puts "---------------------------- Help ----------------------------"
puts "--1.-- Enter Project: "
puts "do pro_tcl.tcl work"

### --2. print the input information
puts " "
puts "do file:   $argv0"
puts "arguments: $1"
puts " "

### --3. execute command
set command $1 

switch $command {
	"first" {
		puts "The fisrt test argc is $command"
	}
	"second" {
		puts "The second test argc is $command"
	}
	default {
		puts "Invaild grade"
	}
}
###-----------------------------------------------------------------
