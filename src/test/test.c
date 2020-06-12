#include "../../include/test.h"
#include "../../include/string.h"
#include <stddef.h>
#include <stdint.h>

static test_case_result test_table_[0XFF];
static uint16_t nmber_test_ = 0;

uint16_t find_test_case(char* test_case)
{
    if (nmber_test_)
        return 0;

    uint8_t i = 0;
    for (i; i < nmber_test_; i++)
        if (_strcmp_(test_case, test_table_[i].test_case_name) == true)
            return (uint16_t)i;

    return (uint16_t)(-1);
}

uint16_t find_unit_test(char* unit_test, test_case_result test_case_)
{
    uint16_t i;

    if (test_case_.nmber_test == 0)
        return 0;

    for (i = 0; i < test_case_.nmber_test; i++)
        if (_strcmp_(test_case_.tests_units[i].test_unit_name, unit_test) == true)
            return i;

    return (uint16_t)(-1);
}

void test_unit(char _test_case_[0xF], char _test_unit[0xF], void* test_function)
{
    uint16_t test_case_index = find_test_case(_test_case_);

    if (nmber_test_ == 0)
        return;

    if (test_case_index == (uint16_t)(-1)) // If the test case doesn't exist
        return;

    uint16_t test_unit_index = find_unit_test(_test_unit, test_table_[test_case_index]);

    if (test_table_[test_case_index].nmber_test != 0) // Verify the zero value of test_unit_index
        if (test_unit_index != (uint16_t)(-1))
            return;

    test_case_result tmp;
    tmp = test_table_[test_case_index]; // Check the test case
    test_unit_result* tmp_test_unit;
    tmp_test_unit = (&(tmp.tests_units[tmp.nmber_test])); // check the unit test a the end

    // Create the unit test
    _strcpy_(tmp_test_unit->test_unit_name, _test_unit);
    tmp_test_unit->test_unit_code = test_function;
    _strcpy_(tmp_test_unit->valid_test.test_case, "");
    _strcpy_(tmp_test_unit->unvalid_test.test_case, "");
    _strcpy_(tmp_test_unit->valid_test.test_unit, "");
    _strcpy_(tmp_test_unit->unvalid_test.test_unit, "");
    tmp_test_unit->unvalid_test.throws_function_ = NULL;
    tmp_test_unit->valid_test.throws_function_ = NULL;

    // Save the unit test
    tmp.nmber_test++;
    test_table_[find_test_case(_test_case_)] = tmp;
}

void test_unit_throws_valid_test(char _test_case_[0xF],
                                 char _test_unit[0xF],
                                 void* test_function,
                                 char _test_case_result[0xF],
                                 char _test_unit_result[0xF],
                                 void* function_result)
{
    uint16_t test_case_index = find_test_case(_test_case_result);

    if (nmber_test_ == 0)
        return;

    if (test_case_index == (uint16_t)(-1)) // If the test case doesn't exist
        return;

    uint16_t test_unit_index =
        find_unit_test(_test_unit_result, test_table_[test_case_index]);

    if (test_table_[test_case_index].nmber_test == 0)
        return;

    if (test_unit_index == (uint16_t)(-1))
        return;

    // Call test unit
    test_unit(_test_case_, _test_unit, test_function);

    // Insert function result
    _strcpy_(test_table_[find_test_case(_test_case_)]
                 .tests_units[find_unit_test(_test_unit, test_table_[test_case_index])]
                 .valid_test.test_unit,
             _test_unit_result);

    _strcpy_(test_table_[find_test_case(_test_case_)]
                 .tests_units[find_unit_test(_test_unit, test_table_[test_case_index])]
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

    if (nmber_test_ == 0)
        return;

    if (test_case_index == (uint16_t)(-1)) // If the test case doesn't exist
        return;

    uint16_t test_unit_index =
        find_unit_test(_test_unit_result, test_table_[test_case_index]);

    if (test_table_[test_case_index].nmber_test == 0)
        return;

    if (test_unit_index == (uint16_t)(-1))
        return;

    // Call test unit
    test_unit(_test_case_, _test_unit, test_function);

    // Insert function result
    _strcpy_(test_table_[find_test_case(_test_case_)]
                 .tests_units[find_unit_test(_test_unit, test_table_[test_case_index])]
                 .unvalid_test.test_unit,
             _test_unit_result);

    _strcpy_(test_table_[find_test_case(_test_case_)]
                 .tests_units[find_unit_test(_test_unit, test_table_[test_case_index])]
                 .unvalid_test.test_case,
             _test_unit_result);
}

void test_case(char* _test_case_)
{
    // Find if the test case exist on the table
    if (nmber_test_ != 0 && find_test_case(_test_case_) != (uint16_t)(-1))
        return;

    _strcpy_(test_table_[nmber_test_].test_case_name, _test_case_);
    test_table_[nmber_test_].nmber_test = 0;
    nmber_test_++;
}
