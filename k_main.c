#define KERNEL__PAGE_MM
#define KERNEL__Vir_MM

#include "include/init/gdt.h"
#include "include/init/idt.h"
#include "include/init/io.h"
#include "include/init/paging.h"
#include "include/init/video.h"
#include "include/lib.h"
#include "include/mm.h"

unsigned int main()
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

    while (1)
        ;

    return 0;
}
