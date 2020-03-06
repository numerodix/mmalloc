#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mmalloc.h"


char userdata[] = "hi there";

int main() {
    // allocate a buffer
    char *buffer = (char*) mmalloc(sizeof(userdata));
    assert(buffer != NULL);

    // fill it with a known contents
    memcpy(buffer, userdata, sizeof(userdata));

    // check that it was filled successfully
    int res = memcmp(buffer, userdata, sizeof(userdata));
    assert(res == 0);

    // de-allocate the buffer
    mfree(buffer);

    return 0;
}