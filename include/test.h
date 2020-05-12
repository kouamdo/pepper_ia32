#ifndef TEST_H

#include "i386types.h"

typedef struct throw_test {
    char* test_case;
    char* test_unit;
} throw_test;

typedef struct tes_unit_result {
    bool passed;                // 0 if the test was failed
    char test_unit_name[30];    // Unit test name
    char test_unit_message[30]; // Unit test message
    void* test_unit_code;       // Code of test unit
    throw_test valid_test;      // Run test if this test was succed
    throw_test unvalid_test;    // Run test if this test was failed
} test_unit_result;

typedef struct test_case_result {
    bool passed; // 0 if the test was failed , 1 if not

    char test_case_name[30]; // Execute test case

    char test_case_message[10][30]; // Message of all unit testing    Each test case should have maximum of 10 units case

    test_unit_result tests_units[10]; // Repertory of unit test

    uint8_t nmber_test : 4; // Number of unit test

} test_case_result;

static test_case_result TEST_SUBSYSTEM[0XFF];
uint8_t NMBER_TEST_CASE = 0;

#define TEST_UNIT(__TEST_CASE__, __TEST_UNIT__, __EXEC_UNIT_TEST__) \
    ({                                                                \
        test_case_result tmp;                                         \
        tmp = TEST_SUBSYSTEM[find_test_case(__TEST_CASE__)];          \
        test_unit_result tmp_test_unit = tmp.tests_units[nmber_test]; \
        tmp_test_unit.test_unit_name = __TEST_UNIT__;                 \
        tmp_test_unit.test_unit_code = (void*)__EXEC_UNIT_TEST__;     \
        tmp_test_unit.valid_test = {"" , ""};   \
        tmp_test_unit.unvalid_test={"" , ""};   \
        tmp.tests_units[nmber_test] = tmp_test_unit;    \
        tmp.nmber_test++ ;  \
        TEST_SUBSYSTEM[find_test_case(__TEST_CASE__)] = tmp;
})

#define TEST_UNIT_THROWS_VALID_TEST(__TEST_CASE__, __TEST_UNIT__, __EXEC_UNIT_TEST__, \
                                    __TEST_CASE_RESULT__, __TEST_UNIT_RESULT__)
({
    TEST_UNIT(__TEST_CASE__, __TEST_UNIT__, __EXEC_UNIT_TEST__);
    TEST_SUBSYSTEM[find_test_case(__TEST_CASE__)].tests_units[nmber_test].valid_test = {
        __TEST_CASE_RESULT__, __TEST_UNIT_RESULT__};   \
})

#define TEST_UNIT_THROWS_UNVALID_TEST(__TEST_CASE__, __TEST_UNIT__, __EXEC_UNIT_TEST__, \
                                      __TEST_CASE_RESULT__, __TEST_UNIT_RESULT__)
({
    TEST_UNIT(__TEST_CASE__, __TEST_UNIT__, __EXEC_UNIT_TEST__);
    TEST_SUBSYSTEM[find_test_case(__TEST_CASE__)].tests_units[nmber_test].unvalid_test = {
        __TEST_CASE_RESULT__, __TEST_UNIT_RESULT__};   \
})

#define TEST_CASE(__TEST_CASE_NAME__)                                        \
    ({                                                                       \
        TEST_SUBSYSTEM[NMBER_TEST_CASE].test_case_name = __TEST_CASE_NAME__; \
        TEST_SUBSYSTEM[NMBER_TEST_CASE].nmber_test = 0;                      \
        NMBER_TEST_CASE++;                                                   \
        \                                                                    \
    })

uint8_t find_unit_test(char* unit_test, test_case_result test_case);
uint8_t find_test_case(char* test_case);

#endif // !TEST_H
