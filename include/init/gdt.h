#ifndef _GDT_H_

#define _GDT_H_

#include "../../include/i386types.h"

#define GDTBASE 0x0  /* addr. physique ou doit resider la gdt */
#define GDTSIZE 0xFF /* nombre max. de descripteurs dans la table */

/*
 *Création d'un descripteur de segment
 */

#define SEG_DESCTYPE(x) \
    ((x) << 0x04) // Description du type (0 for system / 1 for code/data  )
#define SEG_PRIV(x) (((x)&0x03) << 5) // Niveau de privilège (0 - 3)
#define SEG_PRES(x) ((x) << 0x07)     // segment/page présent en mémoire

#define SEG_SAVL(x) ((x) << 0x0C) // Niveau de privilège
#define SEG_LONG(x) ((x) << 0x0D) // Long mode
#define SEG_SIZE(x) ((x) << 0x0E) // Taille (1 pour 16bits / 0 pour 32bits)
#define SEG_GRAN(x) \
    ((x) << 0x0F) // Granularité (0 pour 1o - 1Mo , 1 pour 4Ko - 4Go)

#define SEG_DATA_R 0x00        // Read Only
#define SEG_DATA_R_A 0x01      // Read only , 	accessed
#define SEG_DATA_R_W 0x02      // Read/Write
#define SEG_DATA_R_W_A 0x03    // Read/Write , accessed
#define SEG_DATA_R_EX 0x04     // Read only , expand down
#define SEG_DATA_R_EX_A 0x05   // Read Only , expand down , accessed
#define SEG_DATA_R_W_EX 0x06   // Read/write , expand down
#define SEG_DATA_R_W_EX_A 0X07 // Read/write , expand down , accessed

#define SEG_CODE_E 0x08      // Execute only
#define SEG_CODE_E_A 0x09    // Execute only , accessed
#define SEG_CODE_E_R 0xA     // Execute/read
#define SEG_CODE_E_R_A 0xB   // Execute/read , accessed
#define SEG_CODE_E_C 0xC     // Execute only , conforming
#define SEG_CODE_E_C_A 0xD   // Execute only , conforming , accessed
#define SEG_CODE_E_R_C 0xE   // Execute/Read , conforming
#define SEG_CODE_E_R_C_A 0xF // Execute/read , conforming , accessed

#define GDT_CODE_PL0                                               \
    SEG_CODE_E_R_A | SEG_DESCTYPE(1) | SEG_PRIV(0) | SEG_PRES(1) | \
        SEG_SAVL(1) | SEG_LONG(0) | SEG_SIZE(1) | SEG_GRAN(1)

#define GDT_DATA_PL0                                               \
    SEG_DATA_R_W_A | SEG_DESCTYPE(1) | SEG_PRIV(0) | SEG_PRES(1) | \
        SEG_SAVL(1) | SEG_LONG(0) | SEG_SIZE(1) | SEG_GRAN(1)

#define GDT_STACK_PL0                                                 \
    SEG_DATA_R_W_EX_A | SEG_DESCTYPE(1) | SEG_PRIV(0) | SEG_PRES(1) | \
        SEG_SAVL(1) | SEG_LONG(0) | SEG_SIZE(1) | SEG_GRAN(1)

#define GDT_CODE_PL3                                                 \
    SEG_CODE_E_R_C_A | SEG_DESCTYPE(1) | SEG_PRIV(1) | SEG_PRES(1) | \
        SEG_SAVL(1) | SEG_LONG(0) | SEG_SIZE(1) | SEG_GRAN(1)

#define GDT_DATA_PL3                                               \
    SEG_DATA_R_W_A | SEG_DESCTYPE(1) | SEG_PRIV(1) | SEG_PRES(1) | \
        SEG_SAVL(1) | SEG_LONG(0) | SEG_SIZE(1) | SEG_GRAN(1)

#define GDT_STACK_PL3                                                 \
    SEG_DATA_R_W_EX_A | SEG_DESCTYPE(1) | SEG_PRIV(1) | SEG_PRES(1) | \
        SEG_SAVL(1) | SEG_LONG(0) | SEG_SIZE(1) | SEG_GRAN(1)

#define GDT_TSS                                                                \
    SEG_CODE_E_A | SEG_DESCTYPE(0) | SEG_PRIV(3) | SEG_PRES(1) | SEG_SAVL(0) | \
        SEG_LONG(0) | SEG_SIZE(0) | SEG_GRAN(0)

/* Descripteur de segment */
struct gdtdesc {
    uint16_t lim0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t acces;
    uint8_t lim16_19 : 4;
    uint8_t other : 4;
    uint8_t base24_31;
} __attribute__((packed));

/* Registre GDTR */
unsigned long gdt_ptr[2];

void init_gdt_desc(uint32_t, uint32_t, uint8_t, uint8_t, struct gdtdesc*);
void init_gdt(void);
void EncodeGDTEntry(struct gdtdesc* source, uint8_t* target);

extern void load_gdt(unsigned long* gdtptr);

#ifdef _GDT_H_
struct gdtdesc* kgdt; /* GDT */
#else
extern struct gdtdesc kgdt[];
#endif

#endif // !_GDT_H_
