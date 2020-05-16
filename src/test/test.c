#include "../../include/test.h"
#include <stddef.h>
#include <string.h>

uint16_t find_test_case(char* test_case)
{
    if (NMBER_TEST_CASE == 0)
        return 0;

    uint8_t i = 0;
    for (i; i < NMBER_TEST_CASE; i++)
        if (strcmp(test_case, TEST_SUBSYSTEM[i].test_case_name))
            return i;

    return (uint16_t)(-1);
}

uint16_t find_unit_test(char* unit_test, test_case_result test_case_)
{
    uint16_t i;

    if (test_case_.nmber_test == 0)
        return 0;

    for (i = 0; i < test_case_.nmber_test; i++)
        if (strcmp(test_case_.tests_units[i].test_unit_name, unit_test))
            return i;

    return (uint16_t)(-1);
}

void test_unit(char _test_case_[0xF], char _test_unit[0xF], void* test_function)
{
    uint16_t test_case_index = find_test_case(_test_case_);

    if (NMBER_TEST_CASE == 0)
        return;

    if (test_case_index == (uint16_t)(-1)) // If the test case doesn't exist
        return;

    uint16_t test_unit_index =
        find_unit_test(_test_unit, TEST_SUBSYSTEM[test_case_index]);

    if (TEST_SUBSYSTEM[test_case_index].nmber_test != 0) // Verify the zero value of test_unit_index
        if (test_unit_index != (uint16_t)(-1))
            return;

    test_case_result tmp;
    tmp = TEST_SUBSYSTEM[test_case_index]; // Check the test case
    test_unit_result* tmp_test_unit;
    tmp_test_unit = (&(tmp.tests_units[tmp.nmber_test])); // check the unit test a the end

    // Create the unit test
    strcpy(tmp_test_unit->test_unit_name, _test_unit);
    tmp_test_unit->test_unit_code = test_function;
    strcpy(tmp_test_unit->valid_test.test_case, "");
    strcpy(tmp_test_unit->unvalid_test.test_case, "");
    strcpy(tmp_test_unit->valid_test.test_unit, "");
    strcpy(tmp_test_unit->unvalid_test.test_unit, "");
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
    uint16_t test_case_index = find_test_case(_test_case_result);

    if (NMBER_TEST_CASE == 0)
        return;

    if (test_case_index == (uint16_t)(-1)) // If the test case doesn't exist
        return;

    uint16_t test_unit_index =
        find_unit_test(_test_unit_result, TEST_SUBSYSTEM[test_case_index]);

    if (TEST_SUBSYSTEM[test_case_index].nmber_test == 0)
        return;

    if (test_unit_index == (uint16_t)(-1))
        return;

    // Call test unit
    test_unit(_test_case_, _test_unit, test_function);

    // Insert function result
    strcpy(TEST_SUBSYSTEM[find_test_case(_test_case_)]
               .tests_units[find_unit_test(_test_unit, TEST_SUBSYSTEM[test_case_index])]
               .valid_test.test_unit,
           _test_unit_result);

    strcpy(TEST_SUBSYSTEM[find_test_case(_test_case_)]
               .tests_units[find_unit_test(_test_unit, TEST_SUBSYSTEM[test_case_index])]
               .valid_test.test_case,
           _test_unit_result);
}

void test_unit_throws_unvalid_test(char _test_case_[0xF],
                                   char _test_unit[0xF],
                                   void* test_function,
                                   char _test_case_result[0xF],
                                   char _test_unit_result[0xF],
                                   void* function_result)
{
    uint16_t test_case_index = find_test_case(_test_case_result);

    if (NMBER_TEST_CASE == 0)
        return;

    if (test_case_index == (uint16_t)(-1)) // If the test case doesn't exist
        return;

    uint16_t test_unit_index =
        find_unit_test(_test_unit_result, TEST_SUBSYSTEM[test_case_index]);

    if (TEST_SUBSYSTEM[test_case_index].nmber_test == 0)
        return;

    if (test_unit_index == (uint16_t)(-1))
        return;

    // Call test unit
    test_unit(_test_case_, _test_unit, test_function);

    // Insert function result
    strcpy(TEST_SUBSYSTEM[find_test_case(_test_case_)]
               .tests_units[find_unit_test(_test_unit, TEST_SUBSYSTEM[test_case_index])]
               .unvalid_test.test_unit,
           _test_unit_result);

    strcpy(TEST_SUBSYSTEM[find_test_case(_test_case_)]
               .tests_units[find_unit_test(_test_unit, TEST_SUBSYSTEM[test_case_index])]
               .unvalid_test.test_case,
           _test_unit_result);
}

void test_case(char test_case[0xF])
{
    if (find_test_case(test_case) != (uint16_t)(-1))
        if (NMBER_TEST_CASE != 0)
            return;

    strcpy(TEST_SUBSYSTEM[NMBER_TEST_CASE].test_case_name, test_case);
    TEST_SUBSYSTEM[NMBER_TEST_CASE].nmber_test = 0;
    NMBER_TEST_CASE++;
}