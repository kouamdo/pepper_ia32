#define TEST_H
#include "../../include/test.h"
#include "../../include/string.h"
#include <stddef.h>

static test_case_result test_table_[0XFF];
static int nmber_test_ = 0;

test_case_result* __test_case__handler(char* __test_case__)
{
    if (nmber_test_ == 0)
        return NULL;

    else {
        int i;

        for (i = 0; i < nmber_test_; i++)
            if (_strcmp_(test_table_[i].test_case_name, __test_case__) == true)
                return &test_table_[i];

        return NULL;
    }
}

test_unit_result* __test_unit__handler(char* __test_case__, char* __test_unit__)
{
    test_case_result* __ptr_test_case__ = __test_case__handler(__test_case__);

    if (__ptr_test_case__ == NULL)
        return NULL;

    else if (__ptr_test_case__->nmber_test == 0)
        return NULL;

    else {
        int i;

        for (i = 0; i < __ptr_test_case__->nmber_test; i++)
            if (_strcmp_(__ptr_test_case__->tests_units[i].test_unit_name, __test_unit__) == true)
                return &__ptr_test_case__->tests_units[i];

        return NULL;
    }
}

void create_test_case(char* __test_case__, char* message)
{
    test_case_result* __ptr_test_case__ = __test_case__handler(__test_case__);

    if (__ptr_test_case__ == NULL) {
        _strcpy_(test_table_[nmber_test_].test_case_name, __test_case__);
        test_table_[nmber_test_].nmber_test = 0;
        _strcpy_(test_table_[nmber_test_].test_case_message, message);
        nmber_test_++;
    }
}

void create_test_unit(char* __test_case__, char* __test_unit__, void* _func_, char* message)
{
    test_unit_result* __ptr_test_unit__ = __test_unit__handler(__test_case__, __test_unit__);
    test_case_result* __ptr_test_case__ = __test_case__handler(__test_case__);

    if (__ptr_test_unit__ == NULL) {
        _strcpy_(__ptr_test_case__->tests_units[__ptr_test_case__->nmber_test].test_unit_name,
                 __test_unit__);
        _strcpy_(__ptr_test_case__->tests_units[__ptr_test_case__->nmber_test].test_unit_message,
                 message);

        __ptr_test_case__->tests_units[__ptr_test_case__->nmber_test].test_unit_function = _func_;

        __ptr_test_case__->nmber_test++;
    }
}

void __insert_throw_valid_test__(char* __test_case__, char* __test_unit__, char* __test_case, char* __test_unit)
{
    test_unit_result *__ptr_test_unit__ = __test_unit__handler(__test_case__, __test_unit__),
                     *__ptr_test_unit_throws__ =
                         __test_unit__handler(__test_case, __test_unit);

    if (__ptr_test_unit__ && __ptr_test_unit_throws__)
        __ptr_test_unit__->valid_test = __ptr_test_unit_throws__;

    else
        return;
}

void __insert_throw_unvalid_test__(char* __test_case__,
                                   char* __test_unit__,
                                   char* __test_case,
                                   char* __test_unit)
{
    test_unit_result *__ptr_test_unit__ = __test_unit__handler(__test_case__, __test_unit__),
                     *__ptr_test_unit_throws__ =
                         __test_unit__handler(__test_case, __test_unit);

    if (__ptr_test_unit__ && __ptr_test_unit_throws__)
        __ptr_test_unit__->unvalid_test = __ptr_test_unit_throws__;

    else
        return;
}