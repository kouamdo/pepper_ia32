#include "../../include/string.h"

bool strcmp(char* str1, char* str2)
{
    if (strlen(str2) != strlen(str1))
        return false;

    uint8_t i = 0;

    while (str1[i] == str2[i])
        i++;

    if (strlen(str1) == i)
        return true;
}

uint64_t strlen(char* str)
{
    uint64_t i = 1;
    while (str[i])
        i++;

    return i;
}

void* strcpy(char* dest, char* src)
{
    uint64_t i = 0;

    while (src[i]) {
        dest[i] = src[i];
        i++;
    }

    return (void*)dest;
}