
ENTRY( reset_handler )

MEMORY
{
	flash (rx) : ORIGIN = 0x08000000, LENGTH = 512K
	sram (rwx) : ORIGIN = 0x20000000, LENGTH = 112K
}

SECTIONS
{
	.text : {
		*(.interrupts)
		*(.text*)	/* Program code */
		. = ALIGN(4);
	} >flash

	.rodata : {
		*(.rodata*)
		. = ALIGN(4);
	} >flash

	/* Read-write initialized data. This gets copied from flash at run time */
	.data : {
		_data = .; 
		*(.data*)
		. = ALIGN(4);
		_data_end = .;
	} >sram AT >flash
	
	/* Read-write zero initialized data */
	.bss : {
		_bss = .;
		*(.bss*)
		. = ALIGN(4);
		_bss_end = .;
	} > sram

}

_fdata = LOADADDR(.data);

_stack = ORIGIN(sram) + LENGTH(sram);