#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "mlist.h"
#include "mmalloc.h"


// For some reason accessing memory right at the end of the heap seems
// to fail sometimes, when the allocation is 5-10mb large. Add some padding.
#define PADDING (1 << 10)


void print_trace(size_t val) {
    // Low level output of the allocation size. Have to use `write` since:
    // - printf uses malloc
    // - even putchar interferes with some programs

    // 29 chars + \0
    char buf[30] = "malloc:                     \n";
    char ascii_code_zero = 48;

    int pos = sizeof(buf) - 3;
    while (val >= 10) {
        char digit = val % 10;
        buf[pos--] = ascii_code_zero + digit;
        val = val / 10;
    }
    buf[pos] = ascii_code_zero + val;

    // write to stderr
    write(2, buf, sizeof(buf));
}


void *mmalloc(size_t size) {
#ifdef EXPORT_REAL_API
    print_trace(size);
#endif

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
    void *ptr_new = ptr_current + needed_size + PADDING;

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
    block_t *block_existing = as_block_pointer(ptr);

    // if we've allocated it before it has to be in the used list - remove it
    int res = REMOVE_FROM_USED_LIST(block_existing);
    assert(res == 0);

    // is the block big enough to satisfy this new request?
    if (size <= block_existing->size) {
        // re-add to the used list
        APPEND_TO_USED_LIST(block_existing);

        // return a data pointer
        return get_block_data_pointer(block_existing);
    }

    // it's too small: append it to the free list
    APPEND_TO_FREE_LIST(block_existing);

    // use malloc to get a new block
    void* ptr_new = mmalloc(size);
    if (!ptr_new) {
        return NULL;
    }

    // copy the contents of the existing block into it
    memcpy(ptr_new, ptr, block_existing->size);

    // return a data pointer to the new block
    return ptr_new;
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



#ifdef EXPORT_REAL_API
void *malloc(size_t size) {
    return mmalloc(size);
}

void *calloc(size_t nmemb, size_t size) {
    return mcalloc(nmemb, size);
}

void *realloc(void *ptr, size_t size) {
    return mrealloc(ptr, size);
}

void free(void *ptr) {
    return mfree(ptr);
}
#endif