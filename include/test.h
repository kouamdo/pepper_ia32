#ifndef TEST_H

#include "../i386types.h"
typedef struct tes_unit_result {
    bool passed;
    const char* test_unit_name;
    const char* test_unit_message;
    static void* test_unit_code;
} test_unit_result;

typedef struct test_case_result {
    bool passed; // 0 if the test was failed , 1 if not

    const char* test_case_name; // Execute test case

    const char* test_case_message[10]; // Message of unit testing

    static test_unit_result tests_units[10]; // Repertory of unit test

    uint8_t nmber_test : 4; // Number of unit test

} test_case_result;

static test_unit_result TEST_SUBSYSTEM[0XFF];

#define TEST_UNIT(__TEST_CASE__, __TEST_UNIT__, __EXEC_UNIT_TEST__) \
    TEST_SUBSYSTEM[__TEST_CASE__]

uint8_t find_unit_test(const char* unit_test, test_case_result test_case);
uint8_t find_test_case(const char* test_case);

#endif // !TEST_H
