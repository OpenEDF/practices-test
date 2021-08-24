/* ARM handler for HardFault */
/* Reference Document: ARM®v7-M Architecture Reference Manual */
/* define the debug printf function */
typedef int (* PDEBUG)(fmt, args...);

/* Exception entry behavior */

/* When pushing context to stack, the hardware saves 
 * eight 32-bit words, comprising xPSR, Return Address,
 * LR(R14), R12, R3, R2, R1, and R0.
 */



