.data
    .align 2
back_up_sp:  .word 0
flag:        .word 0

    .text
    .align 2
    .globl  _start
_start:
	li ra,  0x01
	li sp,  0x00002100
	li tp,  0x03
	li t0,  0x04
	li t1,  0x05
	li t2,  0x06
	li s0,  0x07
	li s1,  0x08
	li a0,  0x09
	li a1,  0x0a
	li a2,  0x0b
	li a3,  0x0c
	li a4,  0x0d
	li a5,  0x0e
	li a6,  0x0f
	li a7,  0x10
	li s2,  0x11
	li s3,  0x12
	li s4,  0x13
	li s5,  0x14
	li s6,  0x15
	li s7,  0x16
	li s8,  0x17
	li s9,  0x18
	li s10, 0x19
	li s11, 0x1a
	li t3,  0x1b
	li t4,  0x1c
	li t5,  0x1d
	li t6,  0x0e
	call main

main:
	call push_stack
	nop
	call pop_stack
	j end

push_stack:
	addi sp, sp, -124
	nop
	sw   ra   0(sp)
	sw   sp,  4(sp)
	sw   gp,  8(sp)
	sw   tp,  12(sp)
	sw   t0,  16(sp)
	sw   t1,  20(sp)
	sw   t2,  24(sp)
	sw   s0,  28(sp)
	sw   s1,  32(sp)
	sw   a0,  36(sp)
	sw   a1,  40(sp)
	sw   a2,  44(sp)
	sw   a3,  48(sp)
	sw   a4,  52(sp)
	sw   a5,  56(sp)
	sw   a6,  60(sp)
	sw   a7,  64(sp)
	sw   s2,  68(sp)
	sw   s3,  72(sp)
	sw   s4,  76(sp)
	sw   s5,  80(sp)
	sw   s6,  84(sp)
	sw   s7,  88(sp)
	sw   s8,  92(sp)
	sw   s9,  96(sp)
	sw   s10, 100(sp)
	sw   s11, 104(sp)
	sw   t3,  108(sp)
	sw   t4,  112(sp)
	sw   t5,  116(sp)
	sw   t6,  120(sp)
	la   t0,  back_up_sp
	sw   sp,  0(t0)
	call handler
	nop

pop_stack:
	la   t0   back_up_sp
	lw   sp   0(t0)
	sw   x0,  0(t0)  # lcear sp
	lw   ra   0(sp)
	lw   sp,  4(sp)
	lw   gp,  8(sp)
	lw   tp,  12(sp)
	lw   t0,  16(sp)
	lw   t1,  20(sp)
	lw   t2,  24(sp)
	lw   s0,  28(sp)
	lw   s1,  32(sp)
	lw   a0,  36(sp)
	lw   a1,  40(sp)
	lw   a2,  44(sp)
	lw   a3,  48(sp)
	lw   a4,  52(sp)
	lw   a5,  56(sp)
	lw   a6,  60(sp)
	lw   a7,  64(sp)
	lw   s2,  68(sp)
	lw   s3,  72(sp)
	lw   s4,  76(sp)
	lw   s5,  80(sp)
	lw   s6,  84(sp)
	lw   s7,  88(sp)
	lw   s8,  92(sp)
	lw   s9,  96(sp)
	lw   s10, 100(sp)
	lw   s11, 104(sp)
	lw   t3,  108(sp)
	lw   t4,  112(sp)
	lw   t5,  116(sp)
	lw   t6,  120(sp)
	addi ra, ra, 0x04
	addi sp, sp, 124
	j end

handler:
	li s2, 0x22
	li s3, 0x33
	la t0, flag
	li t1, 0xFFFFABCD
	sw t1, 0(t0)
	call entry

entry:
	la t0, flag
	lw t0, (t0)
	li t1, 0xFFFFABCD
	beq t0, t1, ifif
	j else
else:
	li t0,0x12345678
	j end
ifif:
	la t0, flag
	sw x0, 0(t0)        #clear flag
	la t0, back_up_sp
	lw t1, 0(t0)
	lw t2, 0(t1)
	addi t2, t2, 0x04
	csrw t2, uepc
	uret

end:
	.end

