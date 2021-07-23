// See LICENSE for license details.
#include "encoding.h"

    .section .init
    .globl _start
    .type _start,@function

_start:
    /* Disable all trap in the start process */
    li t0, MSTATUS_MIE
    csrc mstatus, t0
    li t0, MIP_MTIP
    csrc mie, t0
    li t0, MIP_MEIP
    csrc mie, t0
    li t0, 0
    csrc mcause, t0

    .cfi_startproc

    /* Inform the debugger that there is nowhere to backtrace past _enter. */
    .cfi_undefined ra

    /* The absolute first thing that must happen is configuring the global
     * pointer register, which must be done with relaxation disabled because
     * it's not valid to obtain the address of any symbol without GP
     * configured.  The C environment might go ahead and do this again, but
     * that's safe as it's a fixed register. */
.option push
.option norelax
    la gp, _gp
.option pop

1:
    /* There may be pre-initialization routines inside the MBI code that run in
     * C, so here we set up a C environment.  First we set up a stack pointer,
     * which is left as a weak reference in order to allow initialization
     * routines that do not need a stack to be set up to transparently be
     * called. */
    .weak __stack_top
    la sp, __stack_top

    call software_init

#ifndef __riscv_float_abi_soft
    /* Enable FPU */
    li t0, MSTATUS_FS
    csrs mstatus, t0
    csrr t1, mstatus
    and t1, t1, t0
    beqz t1, 1f
    fssr x0
1:
#endif
    /* argc = argv = 0 */
    li a0, 0
    li a1, 0
    call main
1:  j 1b

    .cfi_endproc
