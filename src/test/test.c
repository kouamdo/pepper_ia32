#include "../../include/test.h"
#include "../../include/string.h"
#include <stddef.h>

uint8_t find_test_case(char* test_case)
{
    uint8_t i = 0;
    for (i; i < 0xFF; i++)
        if (strcmp(test_case, TEST_SUBSYSTEM[i].test_case_name))
            return i;

    return NMBER_TEST_CASE + 1;
}

uint8_t find_unit_test(char* unit_test, char* test_case_name)
{
    test_case_result test_case = TEST_SUBSYSTEM[find_test_case(test_case_name)];
    uint8_t i = 0;

    for (i; i < test_case.nmber_test; i++)
        if (strcmp(unit_test, test_case.tests_units[i].test_unit_name))
            return i;

    return test_case.nmber_test + 1;
}

void test_unit(char _test_case_[0xF], char _test_unit[0xF], void* test_function)
{
    test_case_result tmp;
    if ((NMBER_TEST_CASE == 0) || (find_test_case(_test_case_) > NMBER_TEST_CASE) ||
        find_unit_test(_test_unit, _test_case_) >
            TEST_SUBSYSTEM[find_test_case(_test_case_)].nmber_test) // Check if the test cae exist
        __asm__ __volatile__("hlt");

    tmp = TEST_SUBSYSTEM[find_test_case(_test_case_)]; // Check the test case
    test_unit_result* tmp_test_unit;
    tmp_test_unit = (&(tmp.tests_units[tmp.nmber_test])); // check the unit test a the end

    // Create the unit test
    strcpy(tmp_test_unit->test_unit_name, _test_unit, strlen(_test_unit));
    tmp_test_unit->test_unit_code = test_function;
    strcpy(tmp_test_unit->valid_test.test_case, "", strlen(""));
    strcpy(tmp_test_unit->unvalid_test.test_case, "", strlen(""));
    strcpy(tmp_test_unit->valid_test.test_unit, "", strlen(""));
    strcpy(tmp_test_unit->unvalid_test.test_unit, "", strlen(""));
    tmp_test_unit->unvalid_test.throws_function_ = NULL;
    tmp_test_unit->valid_test.throws_function_ = NULL;

    // Save the unit test
    tmp.nmber_test++;
    TEST_SUBSYSTEM[find_test_case(_test_case_)] = tmp;
}

void test_unit_throws_valid_test(char _test_case_[0xF],
                                 char _test_unit[0xF],
                                 void* test_function,
                                 char _test_case_result[0xF],
                                 char _test_unit_result[0xF],
                                 void* function_result)
{
    // Call test unit
    test_unit(_test_case_, _test_unit, test_function);

    // Insert function result
    if ((TEST_SUBSYSTEM[find_test_case(_test_case_result)].nmber_test == 0) ||
        TEST_SUBSYSTEM[find_test_case(_test_case_result)].nmber_test <
            find_unit_test(_test_unit_result, _test_case_result)) // Check if the test case and test unit exist
        __asm__ __volatile__("ret");

    strcpy(TEST_SUBSYSTEM[find_test_case(_test_case_)]
               .tests_units[find_unit_test(_test_unit, _test_case_)]
               .valid_test.test_unit,
           _test_unit_result, strlen(_test_unit_result));

    strcpy(TEST_SUBSYSTEM[find_test_case(_test_case_)]
               .tests_units[find_unit_test(_test_unit, _test_case_)]
               .valid_test.test_case,
           _test_unit_result, strlen(_test_case_result));
}

void test_unit_throws_unvalid_test(char _test_case_[0xF],
                                   char _test_unit[0xF],
                                   void* test_function,
                                   char _test_case_result[0xF],
                                   char _test_unit_result[0xF],
                                   void* function_result)
{
    // Call test unit
    test_unit(_test_case_, _test_unit, test_function);

    // Insert function result
    if ((TEST_SUBSYSTEM[find_test_case(_test_case_)].nmber_test == 0) ||
        TEST_SUBSYSTEM[find_test_case(_test_case_result)].nmber_test <
            find_unit_test(_test_unit_result, _test_case_result))
        __asm__ __volatile__("ret");

    strcpy(TEST_SUBSYSTEM[find_test_case(_test_case_)]
               .tests_units[find_unit_test(_test_unit, _test_case_)]
               .unvalid_test.test_unit,
           _test_unit_result, strlen(_test_unit_result));

    strcpy(TEST_SUBSYSTEM[find_test_case(_test_case_)]
               .tests_units[find_unit_test(_test_unit, _test_case_)]
               .unvalid_test.test_case,
           _test_unit_result, strlen(_test_case_result));
}

// void test_case()