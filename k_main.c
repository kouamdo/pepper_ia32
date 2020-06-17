#include "include/init/gdt.h"
#include "include/init/idt.h"
#include "include/init/io.h"
#include "include/init/paging.h"
#include "include/init/video.h"
// warning: Source file is more recent than executable.

extern char __bss_start, __bss_end;

unsigned int main()
{
    char* i;

    i = &__bss_start;

    while (i != &__bss_end) {
        *i = 0;
        i++;
    }

    pepper_screen();
    cli;

    init_gdt();

    kprintf(2, READY_COLOR, "Chargement des segments\n");

    init_idt();

    create_test_unit("Second_test", "First_unit_test", "first unit test");

    if (DetectPSE32bit & DetectPGE & DetectPAT & DetectMTRR & DetectMSR) {
        kprintf(2, READY_COLOR, "PSE 32bit , PAT, MTRRs and PGE detected \n");

        if (LinearAddress == 0)
            kprintf(2, READY_COLOR,
                    "Linear address with max width 32bits supported ,\n");
        else
            kprintf(2, READY_COLOR,
                    "Linear address with max width 48bits supported ,\n");

        if (PhysicalAddress == 0)
            kprintf(2, READY_COLOR,
                    "Physical address with max width 32bits supported\n");
        else
            kprintf(2, READY_COLOR,
                    "Physical address with width 36bits supported\n");

        init_paging();

        kprintf(2, READY_COLOR, "Enabling 32-bit paging\n");

        kprintf(3, READY_COLOR,
                "Map kernel at physical address [%]\nInitialisation de "
                "4Mo de "
                "memoire physique\n",
                get_phyaddr((virtaddr_t)(main)));

        kprintf(2, READY_COLOR, "Physical memory manager loaded\n");
    }

    sti;
    while (1)
        ;
    return 0;
}
