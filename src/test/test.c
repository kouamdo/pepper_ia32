#include "../../include/test.h"
#include "../../include/string.h"

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