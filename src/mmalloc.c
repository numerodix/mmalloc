#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "malign.h"
#include "mlist.h"
#include "mmalloc.h"
#include "mtrace.h"


// For some reason accessing memory right at the end of the heap seems
// to fail sometimes, when the allocation is 5-10mb large. Add some padding.
#define PADDING (1 << 10)

// System malloc seems to always return addresses that are aligned to 16 bytes.
// The size of `block_t` is 32 bytes so if we align the pointer and add
// the offset needed for `block_t` we'll have the same end result.
#define ALIGNMENT 16

// We need a mutex to synchronize access to pointers read/written from
// different threads.
pthread_mutex_t lock;


void *mmalloc(size_t size) {
    size_t exponent = get_base2_exponent(size);
    size_t size_index = get_size_index(exponent + 1);

    // enter critical section: take lock
    assert(0 == pthread_mutex_lock(&lock));

    // try to re-use a block from the right free list
    block_t *block = pop_from_a_free_list(size_index);
    if (block) {
        // add it to the used list
        PREPEND_TO_USED_LIST(block);

        // exit critical section: release lock
        assert(0 == pthread_mutex_unlock(&lock));

        // return a data pointer
        return get_block_data_pointer(block);
    }

    // look up the current break
    void *ptr_current = sbrk(0);
    assert((size_t) ptr_current != (size_t) -1);

    // align the base of the new allocation
    void *ptr_aligned = (void *) align_location((size_t) ptr_current, ALIGNMENT);
    if (ptr_aligned < ptr_current) {
        ptr_aligned = (void *) (((size_t) ptr_aligned) + ALIGNMENT);
    }

    // round up the size to a power of 2 size
    size_t size_aligned = get_power_of_2(size_index);

    // compute new break
    size_t needed_size = sizeof(block_t) + size_aligned;
    void *ptr_new = (void *) (((size_t) ptr_aligned) + needed_size + PADDING);

    // set the new break
    int res = brk(ptr_new);
    assert(res == 0);

    // create a block and add it to the used list
    block = init_block(ptr_aligned, size_index);
    PREPEND_TO_USED_LIST(block);

    // exit critical section: release lock
    assert(0 == pthread_mutex_unlock(&lock));

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

    // enter critical section: take lock
    assert(0 == pthread_mutex_lock(&lock));

    // compute the size of the existing block
    size_t existing_size = get_power_of_2(block_existing->size_index);

    // if we've allocated it before it has to be in the used list - remove it
    int res = REMOVE_FROM_USED_LIST(block_existing);
    assert(res == 0);

    // is the block big enough to satisfy this new request?
    if (size <= existing_size) {
        // re-add to the used list
        PREPEND_TO_USED_LIST(block_existing);

        // exit critical section: release lock
        assert(0 == pthread_mutex_unlock(&lock));

        // return a data pointer
        return ptr;
    }

    // it's too small: prepend it to the right free list
    prepend_to_a_free_list(block_existing);

    // exit critical section: release lock
    assert(0 == pthread_mutex_unlock(&lock));

    // use malloc to get a new block
    void* ptr_new = mmalloc(size);
    if (!ptr_new) {
        return NULL;
    }

    // copy the contents of the existing block into it
    memcpy(ptr_new, ptr, existing_size);

    // return a data pointer to the new block
    return ptr_new;
}


void mfree(void *ptr) {
    if (!ptr) {
        return;
    }

    // use the pointer to find the block
    block_t *block = as_block_pointer(ptr);

    // enter critical section: take lock
    assert(0 == pthread_mutex_lock(&lock));

    // remove it from the used list
    int res = REMOVE_FROM_USED_LIST(block);
    assert(res == 0);

    // prepend it to the right free list
    prepend_to_a_free_list(block);

    // exit critical section: release lock
    assert(0 == pthread_mutex_unlock(&lock));
}



#ifdef EXPORT_REAL_API
void *malloc(size_t size) {
#ifdef TRACE
    print_trace(F_MALLOC, size);
#endif

    return mmalloc(size);
}

void *calloc(size_t nmemb, size_t size) {
#ifdef TRACE
    print_trace(F_CALLOC, nmemb * size);
#endif

    return mcalloc(nmemb, size);
}

void *realloc(void *ptr, size_t size) {
#ifdef TRACE
    print_trace(F_REALLOC, size);
#endif

    return mrealloc(ptr, size);
}

void free(void *ptr) {
#ifdef TRACE
    print_trace(F_FREE, 0);
#endif

    mfree(ptr);
}
#endif