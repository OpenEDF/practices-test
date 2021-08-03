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
	
gbdata:
	.word 0x11223344
	.byte 0x12 0x34 0x56 0x78

.text
__start:
#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	li sp, 0x10040020
	li a0, 0x12
	li a1, 0x34
	jal MonitorA0A1A2

	jal addrab		#; call the function
	jal MonitorA0A1A2

	la a5, gbdata
	lw a0, 0(a5)
	lw a1, 4(a5)
	jal MonitorA0A1A2

	j Shutdown

#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  

Shutdown:			#; Shutdown Label  
	li a7, 10		#; ends the program with status code 0
	ecall			#; Simulator Function call

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
	
	jal Newline		#; Print the newline
	mv a0, a4
	jr t0			#; return the caller

addrab:
	addi sp, sp, -20
	sw s0, 16(sp)	#; push the frame pointer
	sw ra, 12(sp)	#; push the return link address

	addi s0, sp, 20
	mv a5, a0
	mv a4, a1
	sw a5, -16(s0)	#; save the parameters to stack
	mv a5, a4
	sw a5, -12(s0)

	lw a4, -12(s0) 	#; load the parameters to register
	lw a5, -16(s0)

	add a5, a4, a5	#; add the a4 and a5, store the result to a5
	mv a0, a5		#; set the return parameters
	
	lw s0, 16(sp)	#; pop the s0 and ra from stack
	lw ra, 12(sp)
	
	addi sp, sp, 20	#; free the stack space
	jr ra			#; pc + 4 to caller
	
		
