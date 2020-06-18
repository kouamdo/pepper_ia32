
#ifndef LIB_h

#define LIB_h

#include "i386types.h"

#define cpuid(code)                                            \
    ({                                                         \
        uint32_t edx;                                          \
        __asm__ __volatile__("cpuid" : "=d"(edx) : "a"(code)); \
        edx;                                                   \
    })

#define cpuid_string(code)                                             \
    ({                                                                 \
        uint32_t registers[0x4];                                       \
        __asm__ __volatile__("cpuid"                                   \
                             : "=a"(registers[0]), "=b"(registers[1]), \
                               "=c"(registers[2]), "=d"(registers[3])  \
                             : "a"(code));                             \
        registers;                                                     \
    })

/*
    The machine-check global control MSRs
*/
#define DetectMSR ((cpuid(0x1) & 0x20) >> 5)

#define ReadMSR(MSR)                                                \
    ({                                                              \
        uint32_t _output_[2];                                       \
        __asm__ __volatile__("rdmsr"                                \
                             : "=a"(_output_[0]), "=d"(_output_[1]) \
                             : "c"(MSR));                           \
        _output_;                                                   \
    })

#define SetMSR(MSRinput, _data_)                                       \
    ({                                                                 \
        __asm__ __volatile__("wrmsr" ::"a"((uint32_t)_data_[0]),       \
                             "d"((uint32_t)_data_[1]), "c"(MSRinput)); \
    })

extern char __bss_start, __bss_end;
extern char __test_section__code_start, __test_section__code_end;
extern char __test_section__data_start, __test_section__data_end;

#define CLEAR_BSS_SECTION         \
    ({                            \
        char* i;                  \
        i = &__bss_start;         \
        while (i != &__bss_end) { \
            *i = 0;               \
            i++;                  \
        }                         \
    })

#define CLEAR_TEST_CODE                          \
    ({                                           \
        char* i;                                 \
        i = &__test_section__code_start;         \
        while (i != &__test_section__code_end) { \
            *i = 0;                              \
            i++;                                 \
        }                                        \
    })

#define CLEAR_TEST_DATA                          \
    ({                                           \
        char* i;                                 \
        i = &__test_section__data_start;         \
        while (i != &__test_section__data_end) { \
            *i = 0;                              \
            i++;                                 \
        }                                        \
    })

#endif // !LIB_h
