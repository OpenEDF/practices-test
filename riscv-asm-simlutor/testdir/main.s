	.file	"main.c"
	.option nopic
	.local	c
	.comm	c,4,4
	.data
	.align	3
	.type	array, @object
	.size	array, 32
array:
	.word	0
	.word	1
	.word	2
	.word	3
	.word	4
	.zero	12
	.text
	.align	1
	.globl	adder
	.type	adder, @function
adder:
	addi	sp,sp,-32
	sd	s0,24(sp)
	addi	s0,sp,32
	mv	a5,a0
	mv	a4,a1
	sw	a5,-20(s0)
	mv	a5,a4
	sw	a5,-24(s0)
	lw	a4,-20(s0)
	lw	a5,-24(s0)
	addw	a5,a4,a5
	sext.w	a4,a5
	lui	a5,%hi(c)
	sw	a4,%lo(c)(a5)
	lui	a5,%hi(c)
	lw	a5,%lo(c)(a5)
	mv	a0,a5
	ld	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	adder, .-adder
	.ident	"GCC: (GNU) 7.2.0"
