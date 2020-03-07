
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mmalloc.h"
#include "mlist.h"

#define SIZE 4 << 10

int main() {
    // allocate a buffer
    char *buffer1 = (char*) mmalloc(SIZE);
    assert(buffer1 != NULL);

    // de-allocate the buffer
    mfree(buffer1);

    // allocate another buffer of the same size
    char *buffer2 = (char*) mmalloc(SIZE);
    assert(buffer2 != NULL);

    assert(buffer1 == buffer2);

    // de-allocate the buffer
    mfree(buffer2);

    // allocate yet another buffer of the same size
    char *buffer3 = (char*) mmalloc(SIZE);
    assert(buffer3 != NULL);

    assert(buffer1 == buffer3);

    // de-allocate the buffer
    mfree(buffer3);

    return 0;
}