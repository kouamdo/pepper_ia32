#include "include/init/gdt.h"
#include "include/init/idt.h"
#include "include/init/io.h"
#include "include/init/video.h"
#include "include/lib.h"
// warning: Source file is more recent than executable.

unsigned int main()
{
    CLEAR_BSS_SECTION;

    pepper_screen();
    kprintf(2, LOADING_COLOR,
            "PEPPER_OS \t: https://github.com/kouamdo/pepper_ia32\n");
    cli;

    init_gdt();

    init_idt();

    sti;
    while (1)
        ;

    return 0;
}
