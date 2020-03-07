#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "mmalloc.h"


char userdata[] = "hi there";

int main() {
    void *buffer = mmalloc(sizeof(userdata) / 2);
    assert(buffer != NULL);

    // when called with a bigger size return a chunk big enough
    buffer = mrealloc(buffer, sizeof(userdata));
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