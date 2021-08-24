/* ARM handler for HardFault */
/* Reference Document: ARM®v7-M Architecture Reference Manual */
/* define the debug printf function */
typedef int (* PDEBUG)(fmt, args...);

/* Exception entry behavior */

/* When pushing context to stack, the hardware saves 
 * eight 32-bit words, comprising xPSR, Return Address,
 * LR(R14), R12, R3, R2, R1, and R0.
 */


/*
 *    Original SP  ----> 
 * 4-Byte aligned		 xPSR
 * 						 Return Address
 * 						 LR
 * 						 R12
 * 						 R3
 * 						 R2
 * 						 R1
 * 	  New SP  	 ---->	 R0
 * */

/* code dump struct */
typedef struct {
	uint32_t xPSR;
	uint32_t return_addr;
	uint32_t core_R14;
	uint32_t core_R3;
	uint32_t core_R2;
	uint32_t core_R1;
	uint32_t core_R0;
}code_dump_t;

/*
 * @function   arm_code_dump
 * @brief      when processor entey exception, print the core information.
 * @param[in]  None.
 * @retval     None.
 */

void arm_code_dump(void)
{
	uint32_t core_sp;
	uint32_t core_psp;
	uint32_t core_msp;
	code_dump_ti *core_info;

	/* read the current sp */
	core_sp = ;
	core_psp = ;
	core_msp = ;

	PDEBUG("\r[INFO] SP : 0x%08X.\n", core_sp);
	PDEBUG("\r[INFO] PSP: 0x%08X.\n", core_psp);
	PDEBUG("\r[INFO] MSP: 0x%08X.\n", core_msp);
	if (core_sp == core_psp)
	{
		PDEBUG("\r[INFO] Processor in Handler mode.\n");
	}
	else
	{	
		PDEBUG("\r[INFO] Processor in Thread mode mode.\n");
	}

	/* read the save register for exception save that entry */
	core_info = (code_dump_t *)core_sp;
	
	PDEBUG("\r[INFO] RO          : 0x%08X.\n", core_info.core_R0);
	PDEBUG("\r[INFO] R1          : 0x%08X.\n", core_info.core_R1);
	PDEBUG("\r[INFO] R2          : 0x%08X.\n", core_info.core_R2);
	PDEBUG("\r[INFO] R3          : 0x%08X.\n", core_info.core_R3);
	PDEBUG("\r[INFO] R14         : 0x%08X.\n", core_info.core_R14);
	PDEBUG("\r[INFO] Return Addr : 0x%08X.\n", core_info.return_addr;
	PDEBUG("\r[INFO] xPSR        : 0x%08X.\n", core_info.xPSR;
}
