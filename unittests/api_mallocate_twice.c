#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "mmalloc.h"


uint64_t nums_fst[] = {7, 7, 7, 7, 7, 7, 7};
uint64_t nums_snd[] = {9, 9, 9, 9, 9, 9, 9};

int main() {
    // allocate a buffer
    char *buffer1 = (char*) mmalloc(sizeof(nums_fst));
    assert(buffer1 != NULL);

    // fill the buffer
    memcpy(buffer1, nums_fst, sizeof(nums_fst));

    // allocate a second buffer of the same size
    char *buffer2 = (char*) mmalloc(sizeof(nums_snd));
    assert(buffer2 != NULL);

    // the buffers should have different locations
    assert(buffer1 != buffer2);

    // fill the second buffer with a different contents
    memcpy(buffer2, nums_snd, sizeof(nums_snd));

    // make sure the first buffer wasn't overwritten
    int res = memcmp(buffer1, nums_fst, sizeof(nums_fst));
    assert(res == 0);

    // de-allocate the buffers
    mfree(buffer1);
    mfree(buffer2);

    return 0;
}