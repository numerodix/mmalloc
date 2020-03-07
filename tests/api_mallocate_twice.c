#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mmalloc.h"


int SIZE = 4 << 10;

int main() {
    // allocate two equal sized buffers
    char *buffer1 = (char*) mmalloc(SIZE);
    assert(buffer1 != NULL);

    char *buffer2 = (char*) mmalloc(SIZE);
    assert(buffer2 != NULL);

    // the buffers should have different locations
    assert(buffer1 != buffer2);

    // de-allocate the buffers
    mfree(buffer1);
    mfree(buffer2);

    return 0;
}