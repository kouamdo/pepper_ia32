#define KERNEL__PAGE_MM
#define KERNEL__Vir_MM

#include <init/gdt.h>
#include <init/idt.h>
#include <init/io.h>
#include <init/paging.h>
#include <init/video.h>
#include <lib.h>
#include <mm.h>
#include <task.h>

extern void doIt();

void main()
{
    CLEAR_BSS_SECTION;

    pepper_screen();

    cli;

    kprintf(2, 11, "[\t\t\t\t\t\tK:::Initialize features\t\t\t\t\t]\n\n\n");

    init_gdt();

    init_idt();

    init_paging();

    sti;

    kprintf(2, 11, "\n\n[\t\t\t\t\t\tK:::Load Memory Manager\t\t\t\t\t]\n\n\n");

    init_page_mem_manage();

    init_vmm();

    init_multitasking();

    while (1)
        ;
}
