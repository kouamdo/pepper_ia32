#ifdef TEST_H

#define TEST_H

#include "i386types.h"

// Test Unit Data Structure
typedef struct test_unit_result {
    bool passed;                    // 0 if the test was failed
    char test_unit_name[0xF];       // Unit test name
    char test_unit_message[30];     // Unit test message
    void* test_unit_function;       // Code of test unit
    test_unit_result* valid_test;   // Run test if this test was succed
    test_unit_result* unvalid_test; // Run test if this test was failed
} __attribute__((packed)) test_unit_result;

// Test Case Data Structure
typedef struct test_case_result {
    bool passed; // 0 if the test was failed , 1 if not

    char test_case_name[0xF]; // Execute test case

    char test_case_message[0x30]; // Test Case Message

    test_unit_result tests_units[0xF]; // Repertory of unit test

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
void __throw_unvalid_test__(char* __test_case__,
                            char* __test_unit__,
                            char* __test_case_throws,
                            char* __test_unit_throws);

// Call unit test if test has done a good job
void __throw_valid_test__(char* __test_case__,
                          char* __test_unit__,
                          char* __test_case_throws,
                          char* __test_unit_throws);
#ifdef TEST_MM

#define TEST_UNIT(test_case, test_unit, message, func) \
    create_test_unit(test_case, test_unit, message, func)

#define TEST_CASE (test_case, message) create_test_case(test_case, message)

#endif // TEST_MM

#endif // !TEST_H
