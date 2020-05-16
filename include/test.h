#ifndef TEST_H

#include "i386types.h"

typedef struct throw_test {
    char test_case[0xF];    // Test case result , after making the last test
    char test_unit[0xF];    // Test unit result after making some test
    void* throws_function_; // Test result to execute after making some test
} __attribute__((packed)) throw_test;

typedef struct test_unit_result {
    bool passed;                // 0 if the test was failed
    char test_unit_name[0xF];   // Unit test name
    char test_unit_message[30]; // Unit test message
    void* test_unit_code;       // Code of test unit
    throw_test valid_test;      // Run test if this test was succed
    throw_test unvalid_test;    // Run test if this test was failed
} __attribute__((packed)) test_unit_result;

typedef struct test_case_result {
    bool passed; // 0 if the test was failed , 1 if not

    char test_case_name[0xF]; // Execute test case

    char test_case_message[0xF][30]; // Message of all unit testing    Each test case should have maximum of 10 units case

    test_unit_result tests_units[0xF]; // Repertory of unit test

    uint8_t nmber_test : 4; // Number of unit test

} __attribute__((packed)) test_case_result;

uint16_t find_unit_test(char* unit_test, test_case_result test_case_);
uint16_t find_test_case(char* test_case);

void test_unit(char _test_case_[0xF], char _test_unit[0xF], void* test_function);
void test_unit_throws_valid_test(char _test_case_[0xF],
                                 char _test_unit[0xF],
                                 void* test_function,
                                 char _test_case_result[0xF],
                                 char _test_unit_result[0xF],
                                 void* function_result);

void test_unit_throws_unvalid_test(char _test_case_[0xF],
                                   char _test_unit[0xF],
                                   void* test_function,
                                   char _test_case_result[0xF],
                                   char _test_unit_result[0xF],
                                   void* function_result);

void test_case(char test_case[0xF]);

static test_case_result TEST_SUBSYSTEM[0XFF];
static uint16_t NMBER_TEST_CASE = 0;

#endif // !TEST_H
