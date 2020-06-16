OUTPUT_FORMAT(elf32-i386)
ENTRY(main)

SECTIONS
{
    . = 0x9000;
    .text : {*(.text)}

    .data : {*(.data)}

    .rodata : {*(.rodata)}

    .bss :
    {
        __bss_start = .;
        *(COMMON)
        *(.bss)
        __bss_end = .;
    }

    end = .; _end = .; __end = .;
}
