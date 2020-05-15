#include "../../include/test.h"
#include "../../include/string.h"
#include <stddef.h>

static test_case_result TEST_SUBSYSTEM[0XFF];
uint8_t NMBER_TEST_CASE = 0;

static uint16_t find_test_case(char* test_case)
{
    uint8_t i = 0;
    if (NMBER_TEST_CASE == 0)
        return NMBER_TEST_CASE;
    for (i; i < NMBER_TEST_CASE; i++)
        if (strcmp(test_case, TEST_SUBSYSTEM[i].test_case_name))
            return i;

    return (-1);
}

static uint16_t find_unit_test(char* unit_test, char* test_case_name)
{
    test_case_result test_case = TEST_SUBSYSTEM[find_test_case(test_case_name)];
    uint8_t i = 0;

    if (test_case.nmber_test == 0)
        return 0;

    for (i; i < test_case.nmber_test; i++)
        if (strcmp(unit_test, test_case.tests_units[i].test_unit_name))
            return i;

    return (-1);
}

void test_unit(char _test_case_[0xF], char _test_unit[0xF], void* test_function)
{
    if ((find_test_case(_test_case_) == (uint16_t)(-1)) ||
        (find_unit_test(_test_unit, _test_case_) != (uint16_t)(-1))) // check if the test case exist
        return;

    test_case_result tmp;
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
    if ((find_test_case(_test_case_result) == (uint16_t)(-1)) &&
        (find_unit_test(_test_unit_result, _test_case_result) == (uint16_t)(-1))) // check if the test case exist
        return;

    // Call test unit
    test_unit(_test_case_, _test_unit, test_function);

    // Insert function result
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
    if ((find_test_case(_test_case_result) == (uint16_t)(-1)) &&
        (find_unit_test(_test_unit_result, _test_case_result) == (uint16_t)(-1))) // check if the test case exist
        return;

    // Call test unit
    test_unit(_test_case_, _test_unit, test_function);

    // Insert function result
    strcpy(TEST_SUBSYSTEM[find_test_case(_test_case_)]
               .tests_units[find_unit_test(_test_unit, _test_case_)]
               .unvalid_test.test_unit,
           _test_unit_result, strlen(_test_unit_result));

    strcpy(TEST_SUBSYSTEM[find_test_case(_test_case_)]
               .tests_units[find_unit_test(_test_unit, _test_case_)]
               .unvalid_test.test_case,
           _test_unit_result, strlen(_test_case_result));
}

void test_case(char test_case[0xF])
{
    if (find_test_case(test_case) != (uint16_t)(-1)) // check if the test case exist
        return;

    strcpy(TEST_SUBSYSTEM[NMBER_TEST_CASE].test_case_name, test_case, strlen(test_case));
    TEST_SUBSYSTEM[NMBER_TEST_CASE].nmber_test = 0;
    NMBER_TEST_CASE++;
}