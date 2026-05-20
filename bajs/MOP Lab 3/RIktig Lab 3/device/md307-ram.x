/*
	md307-ram-crt.x
	Linker script for MD307
*/

MEMORY
{
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 108K
}

SECTIONS
{
     .text :
    {
	. = ALIGN(4);	    
	*(.start_section)	/* startup code */
	*(.text*) 			/* remaining code */
        *(.text.*)                 
	*(.rodata*)  		/* read-only data (constants) */
        *(.rodata.*) 
    } >RAM
	. = ALIGN (CONSTANT (COMMONPAGESIZE));
	.data :
    {
	*(.data*)			/* initialised data */
        *(.data.*) 
	*(.sdata*)
	*(.sdata.*) 
	} >RAM
	. = ALIGN (CONSTANT (COMMONPAGESIZE));
    .bss :
    {
	__bss_start__ = .; 
	*(.bss*)			/* unintialised data */
	*(.bss.*)		
	*(.sbss*) 		
	*(.sbss.*) 		
      *(COMMON)
	. = ALIGN(4); 
	__bss_end__ = .; 
	__heap_low = .; 
	. = . + 0x800; 
	__heap_top = .; 
	. = . + 0x400; 
	__stack_top = .;  
   } >RAM
 }
