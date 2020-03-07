#include <assert.h>
#include <stdlib.h>

#include "mmalloc.h"


int main() {
    // trying to free a NULL pointer should be a no-op
    void *ptr = NULL;
    mfree(ptr);

    return 0;
}