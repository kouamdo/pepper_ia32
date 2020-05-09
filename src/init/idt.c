#include "../../include/init/idt.h"
#include "../../include/lib.h"
#include "../../include/init/video.h"
#include "../../include/init/pic.h"
#include "../../include/init/pit.h"

void set_idt(uint16_t selector, uint8_t type, uint64_t offset,
             uint16_t vector) {
  IDT[vector].selector          = selector; // Kernelcode segment offset
  IDT[vector].type_attr         = type;     // Interrupt gate
  IDT[vector].zero              = 0;        // Only zero
  IDT[vector].offset_lowerbits  = (offset & 0xFFFF);
  IDT[vector].offset_higherbits = (offset & 0xFFFF0000) >> 16;
}

void init_idt() {
  // Initialisation du PIT
  Init_PIT(PIT_0, 0xDAAD);

  // On itiialise les intérruptions qu'on va utiliser
  PIC_remap(0x20, 0x28);

  set_idt(0x08, INTGATE, (unsigned long)PIT_handler, 0x20); // IRQ_0
  set_idt(0x08, INTGATE, (unsigned long)irq1, 0x21);
  set_idt(0x08, INTGATE, (unsigned long)irq2, 0x22);
  set_idt(0x08, INTGATE, (unsigned long)irq3, 0x23);
  set_idt(0x08, INTGATE, (unsigned long)irq4, 0x24);
  set_idt(0x08, INTGATE, (unsigned long)irq5, 0x25);
  set_idt(0x08, INTGATE, (unsigned long)irq6, 0x26);
  set_idt(0x08, INTGATE, (unsigned long)irq7, 0x27);
  set_idt(0x08, INTGATE, (unsigned long)irq8, 0x28);
  set_idt(0x08, INTGATE, (unsigned long)irq9, 0x29);
  set_idt(0x08, INTGATE, (unsigned long)irq10, 0x2A);
  set_idt(0x08, INTGATE, (unsigned long)irq11, 0x2B);
  set_idt(0x08, INTGATE, (unsigned long)irq12, 0x2C);
  set_idt(0x08, INTGATE, (unsigned long)irq13, 0x2D);
  set_idt(0x08, INTGATE, (unsigned long)irq14, 0x2E);
  set_idt(0x08, INTGATE, (unsigned long)irq15, 0x2F);

  set_idt(0x08, INTGATE, (unsigned long)__exception_handler__, 0x8);
  set_idt(0x08, INTGATE, (unsigned long)__exception_handler__, 0xA);
  set_idt(0x08, INTGATE, (unsigned long)__exception_handler__, 0xB);
  set_idt(0x08, INTGATE, (unsigned long)__exception_handler__, 0xC);
  set_idt(0x08, INTGATE, (unsigned long)__exception_handler__, 0xD);
  set_idt(0x08, INTGATE, (unsigned long)Paging_fault, 0xE);
  set_idt(0x08, INTGATE, (unsigned long)__exception_handler__, 0x11);
  set_idt(0x08, INTGATE, (unsigned long)__exception_handler__, 0x1E);

  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x0);
  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x1);
  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x2);
  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x3);
  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x4);
  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x5);
  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x6);
  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x7);
  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x9);
  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x10);
  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x12);
  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x13);
  set_idt(0x08, INTGATE, (unsigned long)__exception_no_ERRCODE__, 0x14);

  /* initialisation de la structure pour GDTR */
  unsigned long idt_adress = (unsigned long)IDT;
  idt_ptr[0] =
      (sizeof(struct IDT_entry) * IDTSIZE) + ((idt_adress & 0xFFFF) << 16);
  idt_ptr[1] = idt_adress >> 16;

  kprintf(5, READY_COLOR,
          "Exceptions et interruptions remappees rechargees[int% : int%] "
          "charge\nFrequence d'horloge % \n",
          0x20, 0x2f, IRQ0_frequency);

  load_idt(idt_ptr);
}