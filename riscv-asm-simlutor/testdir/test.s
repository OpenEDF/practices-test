	.file	"test.c"
	.option nopic
	.text
	.align	1
	.globl	func
	.type	func, @function
func:
	addi	sp,sp,-64
	sd	ra,56(sp)
	sd	s0,48(sp)
	addi	s0,sp,64
	mv	a6,a0
	mv	a0,a1
	mv	a1,a2
	mv	a2,a3
	mv	a3,a4
	mv	a4,a5
	mv	a5,a6
	sw	a5,-36(s0)
	mv	a5,a0
	sw	a5,-40(s0)
	mv	a5,a1
	sw	a5,-44(s0)
	mv	a5,a2
	sw	a5,-48(s0)
	mv	a5,a3
	sw	a5,-52(s0)
	mv	a5,a4
	sw	a5,-56(s0)
	li	a5,1
	sw	a5,-20(s0)
	li	a5,2
	sw	a5,-24(s0)
	li	a5,3
	sw	a5,-28(s0)
	lw	a4,-24(s0)
	lw	a5,-20(s0)
	mv	a1,a4
	mv	a0,a5
	call	callsub
	lw	a4,-36(s0)
	lw	a5,-52(s0)
	addw	a5,a4,a5
	sext.w	a5,a5
	mv	a0,a5
	ld	ra,56(sp)
	ld	s0,48(sp)
	addi	sp,sp,64
	jr	ra
	.size	func, .-func
	.ident	"GCC: (GNU) 7.2.0"
