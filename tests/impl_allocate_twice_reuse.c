#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mmalloc.h"


#define SIZE_FST 4 << 10
#define SIZE_SND 5 << 10

int main() {
    // allocate two buffers of different size
    char *buffer1 = (char*) mmalloc(SIZE_FST);
    assert(buffer1 != NULL);

    char *buffer2 = (char*) mmalloc(SIZE_SND);
    assert(buffer2 != NULL);

    // de-allocate them both
    mfree(buffer1);
    mfree(buffer2);

    // allocate a buffer of a size we used before
    char *buffer = (char*) mmalloc(SIZE_SND);
    assert(buffer != NULL);

    // we should be reusing a block we've received before
    assert(buffer == buffer2);

    // de-allocate the buffer
    mfree(buffer);

    return 0;
}