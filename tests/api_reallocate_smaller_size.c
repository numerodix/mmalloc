#include <assert.h>
#include <stdlib.h>

#include "mmalloc.h"


#define SIZE 4 << 10

int main() {
    void *buffer = mmalloc(SIZE);
    assert(buffer != NULL);

    // when called with a smaller size the same pointer is returned
    void *ptr = mrealloc(buffer, SIZE >> 1);
    assert(ptr == buffer);

    return 0;
}