#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "mmalloc.h"


char userdata[] = "hi there";

int main() {
    // when called with NULL realloc is equivalent to malloc
    void *buffer = mrealloc(NULL, sizeof(userdata));
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