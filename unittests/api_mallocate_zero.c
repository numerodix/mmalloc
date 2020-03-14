#include <assert.h>
#include <stdlib.h>

#include "mmalloc.h"


int main() {
    // when trying to allocate zero bytes malloc should return a pointer
    // that free will accept
    void *ptr = mmalloc(0);
    assert(ptr != NULL);

    // de-allocate the buffer
    mfree(ptr);

    return 0;
}