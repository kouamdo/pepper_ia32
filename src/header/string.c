#include "../../include/string.h"

bool strcmp(char* str1, char* str2)
{
    if (strlen(str2) != strlen(str1))
        return false;

    uint8_t i = 0;

    while ((str1[i] == str2[i]) && (i <= strlen(str1)))
        i++;

    if (strlen(str1) == i)
        return true;
}

uint64_t strlen(char* str)
{
    uint64_t i = 0;
    while (str[i] != '\0')
        i++;

    return i;
}

void* strcpy(char* dest, char* src, uint64_t size)
{
    uint64_t i = 0;

    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    return (void*)dest;
}