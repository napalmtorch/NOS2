ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
    _kernel_start = .;

    . = 0x00100000;
	.text BLOCK(4K) : ALIGN(4K)
	{
        *(.multiboot)
		*(.text)
	}
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
        *(.rodata)
	}
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

	_kernel_end = ALIGN(4K);
}