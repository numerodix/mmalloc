#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "mmalloc.h"


char userdata[] = "hi there";

int main() {
    void *buffer = mmalloc(sizeof(userdata));
    assert(buffer != NULL);

    // fill it with a known contents
    memcpy(buffer, userdata, sizeof(userdata));

    // when called with the same size the same pointer is returned
    void *ptr = mrealloc(buffer, sizeof(userdata));
    assert(ptr == buffer);

    // check that the contents haven't changed
    int res = memcmp(ptr, userdata, sizeof(userdata));
    assert(res == 0);

    return 0;
}