#include "../../../include/i386types.h"
#include "../../../include/mm.h"

/*
  Memory space to put our nodes (pages)
*/
static _address_order_track_ MEMORY_SPACES_PAGES[0x400];

/*
  _page_area_track_ is one variable , at the head , it is the linked list
  each element of the list point to his next and to his previos element
  we should integrate a good stuff to insert pages , we a good optimisation
*/

/*/
  this function allows to initiate the physical memory manager
  the manager is called _page_area_track
*/

void init_phymem_manage()
{
    _page_area_track_ = MEMORY_SPACES_PAGES;
    (*_page_area_track_).next_ = END_LIST;
    (*_page_area_track_).previous_ = END_LIST;
    (*_page_area_track_)._address_ = NO_PHYSICAL_ADDRESS;
    (*_page_area_track_).order = 0;

    uint32_t i;

    for (i = 0; i < 0x400; i++)
        MEMORY_SPACES_PAGES[i]._address_ = NO_PHYSICAL_ADDRESS;
}

_address_order_track_ alloc_page(uint32_t order)
{
    if ((*_page_area_track_)._address_ == NO_PHYSICAL_ADDRESS) {
        (*_page_area_track_)._address_ = 0x100000;
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

        if (_page_area_track_->_address_ >= 0x100000 + (PAGE_SIZE * order)) {
            new_page->_address_ = 0x100000;
            new_page->order = order;
            new_page->next_ = _page_area_track_;
            _page_area_track_ = new_page;

            return *new_page;
        }

        tmp = (_page_area_track_);

        // Find a optimal position for the new page
        while (tmp->next_ != END_LIST) {
            if ((tmp->next_->_address_) >=
                (tmp->_address_ + ((tmp->order + order) * PAGE_SIZE)))
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
    if (page.previous_ == END_LIST) {
        page._address_ = NO_PHYSICAL_ADDRESS; // freeing one memory address
        _page_area_track_ = _page_area_track_->next_; // point to the second item
        _page_area_track_->previous_ = END_LIST;
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