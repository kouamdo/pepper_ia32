#define TEST_H
#define TEST_M
#include "../../include/init/gdt.h"
#include "../../include/i386types.h"
#include "../../include/init/video.h"
#include "../../include/lib.h"
#include "../../include/test.h"
#include <stddef.h>

extern test_case_result __gdt_testing__;
void test__gdt();
/*
 * init_desc initialise un descripteur de segment situe en gdt ou en ldt.
 * desc est l'adresse lineaire du descripteur a initialiser.
 */
static void init_gdt_entry(uint32_t base, uint32_t limite, uint8_t access, uint8_t flags, gdt_entry_desc desc)
{
    desc.lim0_15 = (limite & 0xFFFF);
    desc.lim16_19 = (limite & 0xF0000) >> 16;

    desc.base0_15 = (base & 0xFFFF);
    desc.base16_23 = (base & 0xFF0000) >> 16;
    desc.base24_31 = (base & 0xFF000000) >> 24;

    desc.flags = flags;
    desc.acces_byte = access;
}

/*
 * Cette fonction initialise la GDT apres que le kernel soit charge
 * en memoire. Une GDT est deja operationnelle, mais c'est celle qui
 * a ete initialisee par le secteur de boot et qui ne correspond
 * pas forcement a celle que l'on souhaite.
 */
void init_gdt(void)
{
    /* initialisation des descripteurs de segment */
    init_gdt_entry(0x0, 0x0, 0x0, 0x0, __gdt_entry__[0]);

    // Segment de code
    init_gdt_entry(0, 0xFFFFF, CODE_PRIVILEGE_0,
                   SEG_GRANULARITY(1) | SEG_SIZE(1), __gdt_entry__[1]);

    // Segment de donnée
    init_gdt_entry(0, 0xFFFFF, DATA_PRIVILEGE_0, 0, __gdt_entry__[2]);

    // Segment de pile
    init_gdt_entry(0, 0xFFFFF, STACK_PRIVILEGE_0, 0, __gdt_entry__[3]);

    /* Reinitialisation des segments */
    __asm__ __volatile__(
        "   movw $0x10, %ax	\n \
            movw %ax, %ds	\n \
            movw %ax, %es	\n \
            movw %ax, %fs	\n \
            movw %ax, %gs	\n \
            ljmp $0x08, $next	\n \
            next:		\n"); // Long jump after reconfiguration of all segment
    // Allocation de 10ko de pile
    __asm__ __volatile__(
        "movw $0x18, %ax \n \
        movw %ax, %ss \n \
        movl $0x20000, %esp \n  \
        ");

    test__gdt();
}

void test__gdt()
{
    uint32_t i = 0;

    int (*func_ptr)(void) = (void*)__gdt_testing__.tests_units[i]->test_unit_function;
    (*func_ptr)();

    if (__gdt_testing__.tests_units[i]->passed == false)
        kprintf(2, ERROR_COLOR, __gdt_testing__.tests_units[i]->test_unit_name);

    for (i = 1; i < __gdt_testing__.nmber_test; i++) {
        if (__gdt_testing__.tests_units[i]->passed == true)
            kprintf(2, ERROR_COLOR, __gdt_testing__.tests_units[i]->test_unit_name);

        func_ptr = __gdt_testing__.tests_units[i]->test_unit_function;
        (*func_ptr)();
    }
}