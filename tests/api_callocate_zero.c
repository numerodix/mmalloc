#include <assert.h>
#include <stdlib.h>

#include "mmalloc.h"


int main() {
    void *ptr;

    // When trying to allocate zero items or zero sized items,
    // calloc should return NULL
    ptr = mcalloc(0, 16);
    assert(ptr == NULL);

    ptr = mcalloc(10, 0);
    assert(ptr == NULL);

    return 0;
}