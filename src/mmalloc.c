#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "mlist.h"
#include "mmalloc.h"


void *mmalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    // try to re-use a block from the free list
    block_t *block = POP_FROM_FREE_LIST(size);
    if (block) {
        // add it to the used list
        APPEND_TO_USED_LIST(block);

        // return a data pointer
        return get_block_data_pointer(block);
    }

    // look up the current break
    void *ptr_current = sbrk(0);
    assert((size_t) ptr_current != -1);

    // compute new break
    size_t needed_size = size + sizeof(block_t);
    void *ptr_new = ptr_current + needed_size;

    // set the new break
    int res = brk(ptr_new);
    assert(res == 0);

    // create a block and add it to the used list
    block = init_block(ptr_current, size, NULL);
    APPEND_TO_USED_LIST(block);

    // return a data pointer
    return get_block_data_pointer(block);
}


void *mcalloc(size_t nmemb, size_t size) {
    size_t size_needed = nmemb * size;

    void *ptr = mmalloc(size_needed);
    if (!ptr) {
        return NULL;
    }

    memset(ptr, 0, size_needed);

    return ptr;
}


void *mrealloc(void *ptr, size_t size) {
    // if the pointer is null then we have no existing state, we just use malloc
    if (!ptr) {
        return mmalloc(size);
    }

    // if the caller wants to shrink the allocation to zero then we free it
    if (ptr && (size == 0)) {
        mfree(ptr);
        return NULL;
    }

    // use the pointer to find the block
    block_t *block = as_block_pointer(ptr);

    // if we've allocated it before it has to be in the used list - remove it
    int res = REMOVE_FROM_USED_LIST(block);
    assert(res == 0);

    // is the block big enough to satisfy this new request?
    if (size <= block->size) {
        // re-add to the used list
        APPEND_TO_USED_LIST(block);

        // return a data pointer
        return get_block_data_pointer(block);

    // otherwise add it to the free list and allocate a fresh one
    } else {
        // append it to the free list
        APPEND_TO_FREE_LIST(block);

        return mmalloc(size);
    }
}


void mfree(void *ptr) {
    if (!ptr) {
        return;
    }

    // use the pointer to find the block
    block_t *block = as_block_pointer(ptr);

    // remove it from the used list
    int res = REMOVE_FROM_USED_LIST(block);
    assert(res == 0);

    // append it to the free list
    APPEND_TO_FREE_LIST(block);
}