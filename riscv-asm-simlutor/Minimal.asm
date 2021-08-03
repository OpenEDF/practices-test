.globl __start

.data		      	#;Data segment - can only read/write to this area
msg1: 
  	.string "Hello World!"
  	.byte 0			#;Zero Terminated String
msg_a0:
  	.string "a0: "
  	.byte 0
msg_a1:
  	.string "a1: "
	.byte 0

msg_a2:
	.string "a2: "
	.byte 0
.text
__start:
#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	li a0, 0x12
	li a1, 0x34
	li a2, 0x56
	jal MonitorA0A1A2
	
	li a1, 30
	beq a1, a0, Shutdown

#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  

Shutdown:			#; Shutdown Label  
	li a7, 10		#; ends the program with status code 0
	ecall			#; Simulator Function call
	jr ra 

#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Newline:			#; Print the new line
	li a0, 0x0A
	li a7, 11
	ecall
	jr ra

#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PrintSpace:
	li a0, 0x20		#; Print space
	li a7, 11
	ecall
	jr ra

#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MonitorA0A1A2:
	mv t0, ra		#; save the return address

	mv a4, a0
	la a0, msg_a0	#; Print "a0"
	li a7, 4		#; Print string
	ecall	

	mv a0, a4		#; Print a0
	li a7, 34 
	ecall
	
	jal PrintSpace	#; space

	la a0, msg_a1	#; Print a1
	li a7, 4
	ecall
	
	mv a0, a1
	li a7, 34 
	ecall

	jal PrintSpace  #; space

	la a0, msg_a2	#; Print a2
	li a7, 4
	ecall

	mv a0, a2
	li a7, 34 
	ecall
	
	li a0, 20
	li a1, 10
	add a2, a0, a1
	mv a0, a2
	
	jr t0			#; return the caller
	
