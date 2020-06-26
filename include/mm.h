#ifndef _MM_

#include "i386types.h"
#include "init/paging.h"
#include <stddef.h>

#define _MM_

void init_phymem_manage();

typedef struct _address_order_table_ {
    physaddr_t _address_;
    uint32_t order;
    struct _address_order_table_* previous_;
    struct _address_order_table_* next_;
} __attribute__((packed)) _address_order_track_;

#define END_LIST ((_address_order_track_*)NULL)
_address_order_track_ alloc_page(uint32_t order);

_address_order_track_* _page_area_track_;
uint32_t NMBER_PAGES_ALLOC;

/*
    Free pages,
        is a lot of simpler and exists to help remember the order of the
   blocks , to free as one disadvantage of a buddy allocator is that the
   caller has to remember the size of the original allocation.
*/

// Free a page from the given virtual address
void free_page(_address_order_track_ page);

// Reallocate another page with another size
_address_order_track_ calloc_page(_address_order_track_ page, uint32_t order);
#endif