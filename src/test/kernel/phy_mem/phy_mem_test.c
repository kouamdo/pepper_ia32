/*

    *   Test memory allocation
*/

#define TEST_H
#define TEST_M

#include "../../../../include/mm.h"
#include "../../../../include/test.h"
#include <stddef.h>

extern _address_order_track_* _page_area_track_;
extern uint32_t compteur;

TEST_UNIT_FUNC(phy_mem_test_func__1);
TEST_UNIT_FUNC(phy_mem_test_func__2);
TEST_UNIT_FUNC(phy_mem_test_func__3);
TEST_UNIT_FUNC(phy_mem_test_func__4);

TEST_UNIT(phy_mem_test__1) = {

    true,
    "Call alloc page for different chunk of page",
    "Call alloc page for different chunk of page and verify the correct "
    "address",
    &phy_mem_test_func__1,
    NULL,
    NULL};

TEST_UNIT(phy_mem_test__2) = {
    true,
    "Make multiple free page",
    "Make sure that multiple free page with the same address doesn't break",
    &phy_mem_test_func__2,
    NULL,
    NULL};

TEST_UNIT(phy_mem_test__3) = {
    true,
    "Allocate all available memory with the same junk size in the loop",
    "Allocate all available memory with the same junk size in the loop Then "
    "free them in random order, make sure there are no errors.",
    &phy_mem_test_func__3,
    NULL,
    NULL};

TEST_UNIT(phy_mem_test__4) = {
    true,
    "Allocate all available memory with random chunk sizes",
    " Allocate all available memory with random chunk sizes, then free",
    &phy_mem_test_func__4,
    NULL,
    NULL};

TEST_UNIT_FUNC(phy_mem_test_func__1)
{
    uint32_t i;
    phy_mem_test__1.passed = true;

    for (i = 0; i < 0x400; i++)
        alloc_page(i);

    _address_order_track_* tmp;

    tmp = _page_area_track_->next_;

    while (tmp->next_ != END_LIST) {
        if (tmp->_address_ == tmp->previous_->_address_ + (tmp->order * PAGE_SIZE)) {
            phy_mem_test__1.passed = false;
            break;
        }
        tmp = tmp->next_;
    }

    if (phy_mem_test__1.passed == true)
        phy_mem_test_func__2();
}
TEST_UNIT_FUNC(phy_mem_test_func__2)
{
    uint32_t i;

    for (i = 0; i < 0x400; i++)
        free_page(*_page_area_track_);

    if (_page_area_track_->next_ == END_LIST)
        phy_mem_test__2.passed = true;

    else
        phy_mem_test__2.passed = false;
}

TEST_UNIT_FUNC(phy_mem_test_func__3)
{
    if (phy_mem_test__2.passed == false || phy_mem_test__1.passed == false)
        phy_mem_test__3.passed = false;

    uint32_t i;

    // Begin by allocate some page
    for (i = 0; i < 0x400; i++)
        alloc_page(i);

    // Remove it randomly
    for (i = 0; i < 0x400; i++) {
        int j = compteur % (0x400 - i), k = 1;

        _address_order_track_* tmp;

        tmp = _page_area_track_;

        while (tmp->next_ != END_LIST && k < j) {
            tmp = tmp->next_;
            k++;
        }

        free_page(*tmp);
    }

    if (_page_area_track_->_address_ == NO_PHYSICAL_ADDRESS)
        phy_mem_test__3.passed = true;
}
TEST_UNIT_FUNC(phy_mem_test_func__4)
{
    uint32_t i, j;

    for (i = 0; i < 0x400; i++) {
        j = compteur % (5);

        alloc_page(j);
    }

    _address_order_track_* tmp;

    tmp = _page_area_track_;

    while (tmp->next_ != END_LIST) {
        if (tmp->_address_ == tmp->previous_->_address_ + (tmp->order * PAGE_SIZE)) {
            phy_mem_test__4.passed = false;
            break;
        }
        tmp = tmp->next_;
    }
}

TEST_CASE(__phy_mem_manager__) = {true,
                                  "Test physical memory manager",
                                  "Test physical memory manager",
                                  {&phy_mem_test__1, &phy_mem_test__3, &phy_mem_test__4},
                                  3};