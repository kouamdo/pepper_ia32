#include "../include/lib.h"

//Fonction de copie
void memcpy(char *dest, char *src, unsigned int i) {
    while (i--) dest[i] = src[i];
}
