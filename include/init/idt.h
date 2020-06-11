#ifndef IDT_H

#define IDTSIZE 0xFF /* nombre max. de descripteurs dans la table */

#define INTGATE 0x8E /* utilise pour gerer les interruptions */
#define IDTBASE 0x800

unsigned long idt_address;
unsigned long idt_ptr[2];

extern int load_idt(unsigned long* idtptr);

extern /* use IRQ 0 to accurately keep track of
    real time in milliseconds since the PIT was configured .
        cette interruption permettra d'eviter certaines incohérences
        pour la définitionsd e la fréquence
    */
    unsigned long
    PIT_handler(),
    __exception_handler__(), __exception_no_ERRCODE__();

extern void Paging_fault();

// Descripteur de segment:
struct IDT_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
} __attribute__((__packed__));

extern int irq0();
extern int irq1();
extern int irq2();
extern int irq3();
extern int irq4();
extern int irq5();
extern int irq6();
extern int irq7();
extern int irq8();
extern int irq9();
extern int irq10();
extern int irq11();
extern int irq12();
extern int irq13();
extern int irq14();
extern int irq15();

#include "../i386types.h"

#ifdef _IDT_H
struct IDT_entry* IDT;
#else
struct IDT_entry IDT[IDTSIZE];
#endif

void set_idt(uint16_t selector, uint8_t type, uint64_t offset, uint16_t vector);

void init_idt(void);

#endif // !IDT_H