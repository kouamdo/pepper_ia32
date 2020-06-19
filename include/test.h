#ifdef TEST_H

#define TEST_H

#include "i386types.h"

#define __test_frame__code__ __attribute__((section(".test_section__code")))
#define __test_frame__data__ __attribute__((section(".test_section__data")))
// Test Unit Data Structure
typedef struct test_unit_result {
    bool passed;                           // 0 if the test was failed
    char test_unit_name[0xF];              // Unit test name
    char test_unit_message[50];            // Unit test message
    void* test_unit_function;              // Code of test unit
    struct test_unit_result* valid_test;   // Run test if this test was succed
    struct test_unit_result* unvalid_test; // Run test if this test was failed
} __attribute__((packed)) test_unit_result;

// Test Case Data Structure
typedef struct test_case_result {
    bool passed; // 0 if the test was failed , 1 if not

    char test_case_name[0xF]; // Execute test case

    char test_case_message[50]; // Test Case Message

    test_unit_result* tests_units[0xF]; // Repertory of unit test

    uint8_t nmber_test : 4; // Number of unit test

} __attribute__((packed)) test_case_result;

// Test case handler
test_case_result* __test_case__handler(char* __test_case__);

// Test unit handler
test_unit_result* __test_unit__handler(char* __test_case__, char* __test_unit__);

// Create simple test case
void create_test_case(char* __test_case__, char* message);

// Create simple test unit of one test case
void create_test_unit(char* __test_case__, char* __test_unit__, void* _func_, char* message);

// Call unit test if test has lost
void __insert_throw_unvalid_test__(char* __test_case__,
                                   char* __test_unit__,
                                   char* __test_case_throws,
                                   char* __test_unit_throws);

// Call unit test if test has done a good job
void __insert_throw_valid_test__(char* __test_case__,
                                 char* __test_unit__,
                                 char* __test_case_throws,
                                 char* __test_unit_throws);

#ifdef TEST_M
#define TEST_CASE(var) __test_frame__data__ struct test_case_result var
#define TEST_UNIT(var) __test_frame__data__ struct test_unit_result var
#define TEST_UNIT_FUNC(func) __test_frame__code__ void func()
#endif // TEST_M

#endif // !TEST_H
