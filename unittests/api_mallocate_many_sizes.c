#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mmalloc.h"
#include "mtesting.h"


int main() {
    int min_size = 1 << 4;
    int max_size = 10 << 20;

    long seed_val = set_rand48_seed();
    printf("Using seed value: %ld\n", seed_val);

    for (int i = 0; i < 10; i++) {
        int size = get_random_int(min_size, max_size);
        printf("allocating buffer of size: %d bytes\n", size);

        // allocate a buffer
        char *buffer = (char*) mmalloc(size);
        assert(buffer != NULL);

        // zero it
        memset(buffer, 0, size);

        // de-allocate the buffer
        mfree(buffer);
    }

    return 0;
}