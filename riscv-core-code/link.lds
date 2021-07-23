/* --------------- reg type --------------- */
/* --------------- ram type --------------- */
/* --------------- ext_ram type --------------- */
/* --------------- rom type --------------- */
/* --------------- end address type --------------- */
/* DTOP */
/* BT */
/* DSP */
/* FLASH 2M */
/* PSRAM DDR 4M */
/* Image header */
/**
 * flash start ->  +-----------+
 *                 |   SBL     | --> 64K (16 sectors)
 *                 +-----------+
 *                 |   DCP     | --> 256K (64 sectors)
 *                 +-----------+
 *                 |   BCP     | --> 512K (128 sectors)
 *                 +-----------+
 *                 | COREDUMP  | --> 512K (128 sectors)
 *                 +-----------+
 *                 |   TONE    | --> 256K (64 sectors)
 *                 +-----------+
 *                 |   ACP     | --> 384K (96 sectors)
 *                 +-----------+
 *                 |    KV     | --> 32K (8 sectors)
 *                 +-----------+
 *                 | BOOT MAP  | --> 8K (2 sectors)
 *                 +-----------+
 *                 |    OEM    | --> 4K (1 sectors)
 *                 +-----------+
 *                 | RF/AN CALI| --> 4K (1 sectors)
 * flash end   ->  +-----------+
 */
/**
 * DTOP rom total 32K
 */
/**
 * BT rom total 1M (512K*2)
 */
/**
 * DTOP iram total 64K (32K*2 + 64K)
 */
/**
 * BT iram total 288K (128K + 64K*2 + 32K)
 */
/* Share memory */
/* Flash code memory (512K for DTOP) */
/**
 * LOG origin definiation
 * At end of th dcp iram
 * DTOP 4K
 * BT   8K
 * DSP  4K
 */
/* ipc message */
OUTPUT_ARCH( "riscv" )
ENTRY( _start )
__stack_size = DEFINED(__stack_size) ? __stack_size : 4K;
MEMORY
{
    flash : ORIGIN = (0x10200000), LENGTH = (0x18000)
    ram : ORIGIN = ((0x10200000) + (0x18000)), LENGTH = (((0x1020Ffff - 0x10200000 + 1) + (0x1021ffff - 0x10210000 + 1) + (0x1022ffff - 0x10220000 + 1)) - (0x18000))
}
SECTIONS
{
    .init :
    {
        KEEP (*(SORT_NONE(.init)))
    } >flash
    .text :
    {
        *(.text .text.*)
        *(.gnu.linkonce.t.*)
    } >flash
    .fini :
    {
        KEEP (*(SORT_NONE(.fini)))
    } >flash
    PROVIDE (__etext = .);
    PROVIDE (_etext = .);
    PROVIDE (etext = .);
    .rodata :
    {
        *(.rdata)
        *(.rodata .rodata.*)
        *(.gnu.linkonce.r.*)
    } >flash
    . = ALIGN(4);
    .preinit_array :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >flash
    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >flash
    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >flash
    .ctors :
    {
        /* gcc uses crtbegin.o to find the start of
        the constructors, so we make sure it is
        first.  Because this is a wildcard, it
        doesn't matter if the user does not
        actually link against crtbegin.o; the
        linker won't look for a file to match a
        wildcard.  The wildcard also means that it
        doesn't matter which directory crtbegin.o
        is in.  */
        KEEP (*crtbegin.o(.ctors))
        KEEP (*crtbegin?.o(.ctors))
        /* We don't want to include the .ctor section fflash
        the crtend.o file until after the sorted ctors.
        The .ctor section fflash the crtend file contains the
        end of ctors marker and it must be last */
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
    } >flash
    .dtors :
    {
        KEEP (*crtbegin.o(.dtors))
        KEEP (*crtbegin?.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
    } >flash
    .data : ALIGN(8)
    {
        *(.data .data.*)
        *(.gnu.linkonce.d.*)
        PROVIDE( _gp = . + 0x800 );
        *(.srodata.cst16)
        *(.srodata.cst8)
        *(.srodata.cst4)
        *(.srodata.cst2)
        *(.srodata .srodata.*)
        *(.sdata .sdata.*)
        *(.gnu.linkonce.s.*)
        . = ALIGN(4);
        PROVIDE( _single_module_start = .);
        KEEP(*(.singletestmodule.*))
        PROVIDE( _single_module_end = . );
        . = ALIGN(4);
        PROVIDE( _test_module_start = .);
        KEEP(*(.testmodule.*))
        PROVIDE( _test_module_end = . );
    } >ram AT>flash
    PROVIDE( _data_load_addr = LOADADDR(.data) );
    PROVIDE( _data_start = ADDR(.data) );
    PROVIDE( _data_end = ADDR(.data) + SIZEOF(.data) );
    .bss : ALIGN(8)
    {
        *(.sbss*)
        *(.gnu.linkonce.sb.*)
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
    } >ram
    PROVIDE( _bss_start = ADDR(.bss) );
    PROVIDE( _bss_end = ADDR(.bss) + SIZEOF(.bss) );
    .heap :
    {
        . = ALIGN(4);
        PROVIDE( _heap_start = . );
        . = ORIGIN(ram) + LENGTH(ram) - __stack_size;
        PROVIDE( _heap_end = . );
    } >ram
    PROVIDE( _heap_size = _heap_end - _heap_start );
    .stack :
    {
        . += __stack_size;
        PROVIDE( __stack_top = . );
    } >ram
}
