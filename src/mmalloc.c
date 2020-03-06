#include <assert.h>
#include <unistd.h>

#include "mmalloc.h"


struct block {
    size_t sz;
    struct block* next_block;
};


void *mmalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    // look up the current break
    void *ptr_current = sbrk(0);
    assert((size_t) ptr_current != -1);

    // compute new break
    void *ptr_new = ptr_current + size;

    // set the new break
    int res = brk(ptr_new);
    assert(res == 0);

    return ptr_current;
}


void mfree(void *ptr) {

}