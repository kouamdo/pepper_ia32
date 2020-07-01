#include "../../../include/i386types.h"
#include <stdint.h>
#define TEST_H
#define _TEST_

#include "../../../include/init/video.h"
#include "../../../include/mm.h"
#include "../../../include/test.h"
/*
  Memory space to put our nodes (pages)
*/
static _address_order_track_ MEMORY_SPACES_PAGES[0x400]; // Able to take 0x400 pages
extern test_case_result __phy_mem_manager__;

_address_order_track_* _page_area_track_;
uint32_t NMBER_PAGES_ALLOC;

/*
  _page_area_track_ is one variable , at the head , it is the linked list
  each element of the list point to his next and to his previos element
  we should integrate a good stuff to insert pages , we a good optimisation
*/

/*/
  this function allows to initiate the physical memory manager
  the manager is called _page_area_track
*/
/*
    page Table for mapping request of kernel(kernel allocation)
*/
static uint32_t _kernel_table_[PAGE_TABLE_OFFSET]
    __attribute__((aligned(PAGE_TABLE_SIZE)));

void init_phymem_manage()
{
    uint32_t i;
    for (i = 0; i < PAGE_TABLE_OFFSET; i++) {
        _kernel_table_[i] = (i << 12) | (PAGE_PRESENT(1) | PAGE_READ_WRITE |
                                         PAGE_ACCESSED(1) | PAGE_SUPERVISOR);
    }

    create_page_table(_kernel_table_, 1);

    _page_area_track_ = MEMORY_SPACES_PAGES;
    (*_page_area_track_).next_ = END_LIST;
    (*_page_area_track_).previous_ = END_LIST;
    (*_page_area_track_)._address_ = NO_PHYSICAL_ADDRESS;
    (*_page_area_track_).order = 0;

    for (i = 0; i < 0x400; i++)
        MEMORY_SPACES_PAGES[i]._address_ = NO_PHYSICAL_ADDRESS;

    kprintf(2, 15, "[K:PHY_MEM]\tInitialsation of physical memory manager\n");

    __RUN_TEST__(__phy_mem_manager__);
}

_address_order_track_ alloc_page(uint32_t order)
{
    if ((*_page_area_track_)._address_ == NO_PHYSICAL_ADDRESS) {
        (*_page_area_track_)._address_ = KERNEL__PHY_MEM;
        (*_page_area_track_).order = order;
        (*_page_area_track_).next_ = END_LIST;
        (*_page_area_track_).previous_ = END_LIST;
        NMBER_PAGES_ALLOC = 1;
        return (*_page_area_track_);
    }

    else {
        // FInd the free address space
        uint32_t i = 0;
        while (MEMORY_SPACES_PAGES[i]._address_ != NO_PHYSICAL_ADDRESS)
            i++;

        _address_order_track_ *new_page, *tmp;

        new_page = (_address_order_track_*)(&MEMORY_SPACES_PAGES[i]);

        // If the first page doesn't have the first address at the start of the
        // memory chunk

        if (_page_area_track_->_address_ >= KERNEL__PHY_MEM + (PAGE_SIZE * order)) {
            new_page->_address_ = KERNEL__PHY_MEM;
            new_page->order = order;
            new_page->next_ = _page_area_track_;
            _page_area_track_ = new_page;

            return *new_page;
        }

        tmp = (_page_area_track_);

        // Find a optimal position for the new page
        while (tmp->next_ != END_LIST) {
            if ((tmp->next_->_address_) > (tmp->_address_ + ((tmp->order + order) * PAGE_SIZE)))
                break;
            else
                tmp = tmp->next_;
        }

        if (tmp->next_ == END_LIST) {
            new_page->_address_ = (tmp->_address_) + (tmp->order * PAGE_SIZE);
            new_page->order = order;

            new_page->next_ = END_LIST;
            new_page->previous_ = tmp;

            tmp->next_ = new_page;

            return *new_page;
        }

        else {
            new_page->_address_ = (tmp->_address_) + (tmp->order * PAGE_SIZE);
            new_page->order = order;

            new_page->next_ = tmp->next_;
            new_page->previous_ = tmp;
            tmp->next_ = new_page;
            tmp->next_->previous_ = new_page;

            return *new_page;
        }
        NMBER_PAGES_ALLOC++;
    }
}

void free_page(_address_order_track_ page)
{
    // If it is the head of the list
    if (page.previous_ == END_LIST && page.next_ != END_LIST) {
        page._address_ = NO_PHYSICAL_ADDRESS; // freeing one memory address
        _page_area_track_ = _page_area_track_->next_; // point to the second item
        _page_area_track_->previous_ = END_LIST;
    }

    // If we have only one page in the list
    else if (page.previous_ == END_LIST && page.next_ == END_LIST) {
        _page_area_track_ = MEMORY_SPACES_PAGES;
        (*_page_area_track_).next_ = END_LIST;
        (*_page_area_track_).previous_ = END_LIST;
        (*_page_area_track_)._address_ = NO_PHYSICAL_ADDRESS;
        (*_page_area_track_).order = 0;

        for (uint32_t i = 0; i < 0x400; i++)
            MEMORY_SPACES_PAGES[i]._address_ = NO_PHYSICAL_ADDRESS;
    }

    else {
        _address_order_track_* tmp;

        tmp = _page_area_track_;

        while (tmp->_address_ != page._address_)
            tmp = tmp->next_;

        if (tmp->next_ == END_LIST) // At the end of the list
        {
            tmp->_address_ = NO_PHYSICAL_ADDRESS;
            tmp->previous_->next_ = END_LIST;
        }

        else // At the middle
        {
            tmp->_address_ = NO_PHYSICAL_ADDRESS;
            tmp->previous_->next_ = tmp->next_;
            tmp->next_->previous_ = tmp->previous_;
        }
    }
    NMBER_PAGES_ALLOC--;
}

_address_order_track_ calloc_page(_address_order_track_ page, uint32_t order)
{
    _address_order_track_ new_page;

    new_page = alloc_page(order);

    free_page(page);

    return new_page;
}