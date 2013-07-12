OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(__start)

SECTIONS
{
	. = 0x00000000;
	. = ALIGN(4);
	.text	:
	{
	  *(.start)
	  *(.text)
	}

	.rodata : { *(.rodata) }
	. = ALIGN(4);
	.data : { *(.data) }
	. = ALIGN(4);
	.got : { *(.got) }
	__end = .;

	. = 0x04000100;
	. = ALIGN(4);
	__bss_start = .;
	.bss : { *(.bss) }
	__bss_end = .;

        _lm_heap_start = DEFINED(_lm_heap_start) ? _lm_heap_start : __bss_end;
	/*
	 * FIXME hard code here
	 */
        _lm_heap_end = DEFINED(_lm_heap_end) ? _lm_heap_end : 0x04008000;

	. = ALIGN(4);
	.stack (NOLOAD)	: { *(.stack) }
}
