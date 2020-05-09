#include "../../include/init/io.h"
#include "../../include/init//pic.h"

void irq0_handler(void) {
    PIC_sendEOI(0);
}

void irq1_handler(void) {
    PIC_sendEOI(1);
}

void irq2_handler(void) {
    PIC_sendEOI(2);
}

void irq3_handler(void) {
    PIC_sendEOI(3);
}

void irq4_handler(void) {
    PIC_sendEOI(4);
}

void irq5_handler(void) {
    PIC_sendEOI(5);
}

void irq6_handler(void) {
    PIC_sendEOI(6);
}

void irq7_handler(void) {
    PIC_sendEOI(7);
}

void irq8_handler(void) {
    PIC_sendEOI(8);
}

void irq9_handler(void) {
    PIC_sendEOI(9);
}

void irq10_handler(void) {
    PIC_sendEOI(10);
}

void irq11_handler(void) {
    PIC_sendEOI(11);
}

void irq12_handler(void) {
    PIC_sendEOI(12);
}

void irq13_handler(void) {
    PIC_sendEOI(13);
}

void irq14_handler(void) {
    PIC_sendEOI(14);
}

void irq15_handler(void) {
    PIC_sendEOI(15);
}