#include <assert.h>
#include <unistd.h>

#include "mmalloc.h"
#include "mlist.h"


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


void mfree(void *ptr) {
    if (!ptr) {
        return;
    }

    // used the pointer to find the block
    block_t *block = as_block_pointer(ptr);

    // remove it from the used list
    int res = REMOVE_FROM_USED_LIST(block);
    assert(res == 0);

    // append it to the free list
    APPEND_TO_FREE_LIST(block);
}