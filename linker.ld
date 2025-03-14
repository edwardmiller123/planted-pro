
ENTRY( reset_handler )

MEMORY
{
	flash (rx) : ORIGIN = 0x08000000, LENGTH = 20K
	sram (rwx) : ORIGIN = 0x20000000, LENGTH = 112K
}

SECTIONS
{
	.text : {
		*(.interrupts)
		*(.text*)	/* Progsram code */
		. = ALIGN(4);
		_rodata = .;
		*(.rodata*)	/* Read-only data. */
		_fdata = .; 
		*(.data*) /* This gets copied to sram by the reset handler on startup */
		. = ALIGN(4);
		_fdata_end = .;
	} >flash

	/* Read-write initialized data */
	.data : {
		_data = .; 
		*(.data*)
		. = ALIGN(4);
		_data_end = .;
	} >sram AT>flash
	
	/* Read-write zero initialized data */
	.bss : {
		_bss = .;
		*(.bss*)
		. = ALIGN(4);
	} >sram AT>flash

}

PROVIDE(_stack = ORIGIN(sram) + LENGTH(sram));