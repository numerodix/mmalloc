#include <assert.h>
#include <stdlib.h>

#include "mmalloc.h"


#define SIZE 4 << 10

int main() {
    void *buffer = mmalloc(SIZE);
    assert(buffer != NULL);

    // when called with a non-NULL pointer and size 0 realloc
    // is equivalent to free(buffer)
    void *ptr = mrealloc(buffer, 0);
    assert(ptr == NULL);

    return 0;
}