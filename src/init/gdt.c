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
void init_gdt_desc(uint32_t base, uint32_t limite, uint8_t acces, uint8_t other, struct gdtdesc* desc)
{
    desc->lim0_15 = (limite & 0xffff);
    desc->base0_15 = (base & 0xffff);
    desc->base16_23 = (base & 0xff0000) >> 16;
    desc->acces = acces;
    desc->lim16_19 = (limite & 0xf0000) >> 16;
    desc->other = (other & 0xf);
    desc->base24_31 = (base & 0xff000000) >> 24;
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
    init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);

    // Configuration code, noyau, pile système
    init_gdt_desc(0x0, 0xFFFFFF,
                  SEG_CODE_E_R_A | SEG_DESCTYPE(1) | SEG_PRIV(0) | SEG_PRES(1),
                  0x0D, &kgdt[1]); /* code */

    init_gdt_desc(0x0, 0xFFFFFF,
                  SEG_DATA_R_W_A | SEG_DESCTYPE(1) | SEG_PRIV(0) | SEG_PRES(1),
                  0x0D, &kgdt[2]); /* data */

    init_gdt_desc(0x0, 0xFFFFFF,
                  SEG_DATA_R_W_EX_A | SEG_DESCTYPE(1) | SEG_PRIV(0) | SEG_PRES(1),
                  0x0D, &kgdt[3]); /*Pour la pile*/

    /* initialisation de la structure pour GDTR */
    uint64_t gdt_adress = (uint32_t)kgdt;
    gdt_ptr[0] = (sizeof(struct gdtdesc) * GDTSIZE) + ((gdt_adress & 0xFFFF) << 16);
    gdt_ptr[1] = gdt_adress >> 16;

    kprintf(5, READY_COLOR, "Segments memoire CS,DS,SS [%,%,%]\n",
            SEG_CODE_E_R_A, SEG_DATA_R_W_A, SEG_DATA_R_W_EX_A);
    load_gdt(gdt_ptr);

    /* Reinitialisation des segments */
    __asm__ __volatile__(
        "   movw $0x10, %ax	\n \
            movw %ax, %ds	\n \
            movw %ax, %es	\n \
            movw %ax, %fs	\n \
            movw %ax, %gs	\n \
            ljmp $0x08, $next	\n \
            next:		\n");
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

    int (*func_ptr)(void) = (void*)__gdt_testing__.tests_units[0]->test_unit_function;
    (*func_ptr)();

    if (__gdt_testing__.tests_units[i]->passed == false)
        kprintf(2, ERROR_COLOR, __gdt_testing__.tests_units[i]->test_unit_name);

    // for (i = 1; i < __gdt_testing__.nmber_test; i++) {
    //     if (__gdt_testing__.tests_units[i]->passed == true)
    //         kprintf(2, ERROR_COLOR, __gdt_testing__.tests_units[i]->test_unit_name);

    //     func_ptr = __gdt_testing__.tests_units[i]->test_unit_function;
    //     (*func_ptr)();
    // }
}