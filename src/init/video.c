#include "../../include/init/video.h"
#include <stdarg.h>

void volatile pepper_screen()
{
    volatile unsigned char* screen = (unsigned char*)(VIDEO_MEM);
    int i = 0;
    while (i != 160 * 24) {
        screen[i] = ' ';
        screen[i + 1] = 0x0;
        i += 2;
    }
}

void print_frequence(unsigned int freq)
{
    volatile unsigned char* pos = (unsigned char*)(VIDEO_MEM + 160 * 25 - 10);
    unsigned char i = 10;

    while (i > 0) {
        freq %= 10;
        pos[i] = (freq) + 0x30;
        pos[i + 1] = ADVICE_COLOR;
        i -= 2;
    }
}

void kprintf(int nmber_param, ...)
{
    int val = 0;
    va_list ap;
    int i, tab_param[nmber_param];

    va_start(ap, nmber_param);

    for (i = 0; i < nmber_param; i++) {
        tab_param[i] = va_arg(ap, int);
    }

    unsigned char color = (unsigned char)tab_param[0];
    char* string;

    string = (char*)tab_param[1];

    unsigned char j = 2;

    while (*string) {
        if (*string == '%') {
            print_address(color, tab_param[j]);
            j++;
        }
        else
            putchar(color, *string);

        string++;
    }

    va_end(ap);
}

// void volatile write_string(unsigned char colour, const char string[40]) {
//     if (CURSOR_Y >= 25) {
//         scrollup();
//         CURSOR_X = 0;
//         CURSOR_Y = 0;
//     }

//     volatile unsigned char *vid;

//     while (*string != 0) {
//         vid = (unsigned char *)(VIDEO_MEM + 160 * CURSOR_Y + 2 * CURSOR_X);
//         if (*string == 10) {
//             CURSOR_X = 0;
//             CURSOR_Y++;
//             string++;
//         } else {
//             *(vid) = *string;
//             *(vid + 1) = colour;
//             string++;
//             vid += 2;

//             CURSOR_X++;
//         }
//     }
// }

void volatile scrollup()
{
    unsigned volatile char* v = (unsigned char*)(VIDEO_MEM + 3840);
    unsigned volatile char b[160];
    int i;
    for (i = 0; i < 160; i++)
        b[i] = v[i];

    pepper_screen();

    v = (unsigned char*)(VIDEO_MEM);

    for (i = 0; i < 160; i++)
        v[i] = b[i];

    CURSOR_Y++;
}

void volatile putchar(unsigned char color, unsigned const char c)
{
    if (c != 10) {
        if (CURSOR_Y > 25) {
            scrollup();
            CURSOR_X = 0;
            CURSOR_Y = 0;
        }
        unsigned char* v = (unsigned char*)(VIDEO_MEM + CURSOR_X * 2 + 160 * CURSOR_Y);

        if (c == 10 || CURSOR_X == 80) {
            CURSOR_X = 0;
            CURSOR_Y++;
            *(v) = c;
            *(v + 1) = color;
            CURSOR_X++;
        }
        else {
            *(v) = c;
            *(v + 1) = color;
            CURSOR_X++;
        }
    }

    else {
        CURSOR_X = 0;
        CURSOR_Y++;
    }
}

void volatile print_address(unsigned char color, unsigned int adress__)
{
    char p[10] = {0};

    p[0] |= '0';
    p[1] |= 'x';

    if (CURSOR_Y >= 25) {
        scrollup();
        CURSOR_X = 0;
        CURSOR_Y = 0;
    }

    else {
        unsigned int i, c;

        c = adress__;

        for (i = 1; i <= 8; i++) {
            adress__ = c % 16;
            c /= 16;

            if (adress__ == 15)
                p[10 - i] |= 'F';

            else if (adress__ == 14)
                p[10 - i] |= 'E';

            else if (adress__ == 13)
                p[10 - i] |= 'D';

            else if (adress__ == 12)
                p[10 - i] |= 'C';

            else if (adress__ == 11)
                p[10 - i] |= 'B';

            else if (adress__ == 10)
                p[10 - i] |= 'A';

            else
                p[10 - i] |= (char)(adress__ + 0x30);
        }
        for (i = 0; i < 10; i++)
            putchar(color, p[i]);
    }
}
