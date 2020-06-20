OUTPUT_FORMAT(elf32-i386)
ENTRY(main)

SECTIONS
{
    . = 0x9000;
    .text : {
        *(.text);
        __text__start = .;
        *(.text_code)
        __text__end = .;
    }

    .test_section__code :
    {
        __test_section__code_start = .;
        *(.test_section__code)
        __test_section__code_end = .;
    }

    .test_section__data :
    {
        __test_section__data_start = .;
        *(.test_section__data)
        __test_section__data_end = .;
    }

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
