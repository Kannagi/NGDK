/* 
 * Linker script to generate an ELF file
 * 
 */




ENTRY("_start")




SECTIONS
{
	. = 0x00000000;

	__text_start = .;
	.text ALIGN(2) : { *(.text*) }
	__text_end = .;


	
	__bss_start = 0x100020;
	.bss  ALIGN(1) : { *(.bss) }
	__bss_end = .;


}

