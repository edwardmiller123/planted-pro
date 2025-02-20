
ENTRY( reset_handler )

MEMORY
{
	rom (rx) : ORIGIN = 0x08000000, LENGTH = 64K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
	.text : {
		*(.interrupts)
		*(.text*)	/* Program code */
		. = ALIGN(4);
		*(.rodata*)	/* Read-only data */
		. = ALIGN(4);
	} >rom

}

PROVIDE(_stack = ORIGIN(ram) + LENGTH(ram));