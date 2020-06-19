/*


All testing funtionnaity for gdt segments should be implemennt here

It is more complicated to test the basic descriptor for OS code OS data OS Stack

*/

// GDT testing functionnality

#define TEST_H
#define TEST_M

#include "../../../include/string.h"
#include "../../../include/test.h"
#include <stddef.h>

extern char* gdt_ptr;

TEST_UNIT_FUNC(gdt_testing_func__1_);
TEST_UNIT_FUNC(gdt_testing_func__2_);
TEST_UNIT_FUNC(gdt_testing_func__3_);
TEST_UNIT(__gdt_testing_1_);
TEST_UNIT(__gdt_testing_2_);
TEST_UNIT(__gdt_testing_3_);

// try to verify the ceonservation of the GDT entries

TEST_UNIT(__gdt_testing_1_) = {true, "test", "test", &gdt_testing_func__1_,
                               NULL, NULL};

TEST_UNIT_FUNC(gdt_testing_func__1_)
{
    uint64_t gdtr;

    asm volatile("sgdt %0" : "=m"(gdtr)::"memory");

    if (_memcmp_(gdt_ptr, &gdtr, sizeof(gdtr)) == false)
        __gdt_testing_1_.passed = false;

    else
        __gdt_testing_1_.passed = true;
}

// Try to go beyond the limit expected , test DS segment

TEST_UNIT(__gdt_testing_2_) = {true, "test", "test", &gdt_testing_func__2_,
                               NULL, NULL};

TEST_UNIT_FUNC(gdt_testing_func__2_)
{
    char* p;
    p = (char*)0xFFFFFFFF;

    *p = '\000';
}

// Try to go beyond the CS limit with RETF
TEST_UNIT(__gdt_testing_3_) = {true, "test", "test", &gdt_testing_func__3_,
                               NULL, NULL};
TEST_UNIT_FUNC(gdt_testing_func__3_)
{
    asm volatile(
        "push $0xFFFFFF\n\t"
        "push $0x0\n\t"
        "pop %edx\n\t"
        "pop %es\n\t"
        "retf\n\t");
}

TEST_CASE(__gdt_testing__) = {true,
                              "GDT testing\n\t",
                              "Test each segment entries\n\t",

                              {

                                  &__gdt_testing_1_,
                                  &__gdt_testing_2_,
                                  &__gdt_testing_3_,

                              },

                              3

};