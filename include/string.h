

/*
    string.h

    *Definitions for string functions
*/

#ifndef _STRING_H_

#define _STRING_H_

#include "i386types.h"

bool strcmp(char* str1, char* str2); // Return 0 if st1 os the same as str2
void* strcpy(char* dest, char* src, uint64_t size); // Copies src into dest
uint64_t strlen(char* str);                         // Gives the length of str

#endif // !_STRING_H
