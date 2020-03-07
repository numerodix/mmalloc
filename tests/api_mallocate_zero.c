#include <assert.h>
#include <stdlib.h>

#include "mmalloc.h"


int main() {
    // when trying to allocate zero bytes malloc should return NULL
    void *ptr = mmalloc((size_t) 0);
    assert(ptr == NULL);

    return 0;
}