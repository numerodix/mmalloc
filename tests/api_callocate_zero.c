#include <assert.h>
#include <stdlib.h>

#include "mmalloc.h"


int main() {
    void *ptr;

    // When trying to allocate zero items or zero sized items,
    // calloc should return a pointer that can be passed to free.
    ptr = mcalloc(0, 16);
    assert(ptr != NULL);
    mfree(ptr);

    ptr = mcalloc(10, 0);
    assert(ptr != NULL);
    mfree(ptr);

    return 0;
}