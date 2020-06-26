/*

    *   Test memory allocation
*/

#define TEST_H
#define TEST_M

#include "../../../../include/test.h"
#include <stddef.h>

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
    "Make ultiple free page",
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

TEST_CASE(test_paging_manager) = {
    true,
    "Test physical memory manager",
    "Test physical memory manager",
    {&phy_mem_test__1, &phy_mem_test__2, &phy_mem_test__3, &phy_mem_test__4},
    4};