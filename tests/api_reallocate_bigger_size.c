#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "mmalloc.h"


char userdata_small[] = "hi there";
char userdata[] = "hi there, champ!";

int main() {
    int res;

    void *buffer_small = mmalloc(sizeof(userdata_small));
    assert(buffer_small != NULL);

    // fill it with a known contents
    memcpy(buffer_small, userdata_small, sizeof(userdata_small));

    // when called with a bigger size return a chunk big enough
    void *buffer = mrealloc(buffer_small, sizeof(userdata));
    assert(buffer != NULL);

    // check that the contents in the smaller chunk haven't changed
    res = memcmp(buffer, userdata_small, sizeof(userdata_small));
    assert(res == 0);

    // fill it with a known contents
    memcpy(buffer, userdata, sizeof(userdata));

    // check that it was filled successfully
    res = memcmp(buffer, userdata, sizeof(userdata));
    assert(res == 0);

    // de-allocate the buffer
    mfree(buffer);

    return 0;
}