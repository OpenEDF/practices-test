	.file	"globalvar.c"
	.option nopic
	.data
	.align	3
	.type	array, @object
	.size	array, 32
array:
	.word	1
	.word	2
	.word	3
	.word	4
	.zero	16
	.text
	.align	1
	.globl	addr
	.type	addr, @function
addr:
	addi	sp,sp,-16
	sd	s0,8(sp)
	addi	s0,sp,16
	lui	a5,%hi(array)
	lw	a4,%lo(array)(a5)
	lui	a5,%hi(array)
	addi	a5,a5,%lo(array)
	lw	a5,4(a5)
	addw	a5,a4,a5
	sext.w	a5,a5
	mv	a0,a5
	ld	s0,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	addr, .-addr
	.ident	"GCC: (GNU) 7.2.0"
