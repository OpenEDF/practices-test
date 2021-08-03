	.file	"var.c"
	.option nopic
	.text
	.align	1
	.globl	test
	.type	test, @function
test:
	addi	sp,sp,-48
	sd	s0,40(sp)
	addi	s0,sp,48
	mv	a5,a0
	mv	a4,a1
	sw	a5,-36(s0)
	mv	a5,a4
	sw	a5,-40(s0)
	lw	a4,-36(s0)
	lw	a5,-40(s0)
	addw	a5,a4,a5
	sext.w	a5,a5
	lw	a4,-20(s0)
	addw	a5,a4,a5
	sw	a5,-24(s0)
	lw	a5,-24(s0)
	mv	a0,a5
	ld	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	test, .-test
	.ident	"GCC: (GNU) 7.2.0"
