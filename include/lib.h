
#ifndef LIB_h

#define LIB_h

#include "i386types.h"

//fonction de copie de n octet
void memcpy(int8_t *dest, int8_t *src, uint32_t i);

#define cpuid(code) ({                 \
    uint32_t edx;                      \
    __asm__ __volatile__("cpuid"       \
                         : "=d"(edx)   \
                         : "a"(code)); \
    edx;                               \
})

#define cpuid_string(code) ({                  \
    uint32_t registers[0x4];                   \
    __asm__ __volatile__("cpuid"               \
                         : "=a"(registers[0]), \
                           "=b"(registers[1]), \
                           "=c"(registers[2]), \
                           "=d"(registers[3])  \
                         : "a"(code));         \
    registers;                                 \
})

/*
    The machine-check global control MSRs 
*/
#define DetectMSR ((cpuid(0x1) & 0x20) >> 5)

#define ReadMSR(MSR) ({                                         \
    uint32_t _output_[2];                                       \
    __asm__ __volatile__("rdmsr"                                \
                         : "=a"(_output_[0]), "=d"(_output_[1]) \
                         : "c"(MSR));                           \
    _output_;                                                   \
})

#define SetMSR(MSRinput, _data_) ({ __asm__ __volatile__("wrmsr" ::                    \
                                                             "a"((uint32_t)_data_[0]), \
                                                         "d"((uint32_t)_data_[1]), "c"(MSRinput)); })

#endif  // !LIB_h
