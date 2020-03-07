#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "mmalloc.h"


int main() {
    uint64_t nums[] = {0, 0, 0, 0, 0};

    // allocate a buffer
    char *buffer = (char*) mcalloc(5, sizeof(uint64_t));
    assert(buffer != NULL);

    // check that it was filled successfully
    int res = memcmp(buffer, nums, sizeof(nums));
    assert(res == 0);

    // de-allocate the buffer
    mfree(buffer);

    return 0;
}