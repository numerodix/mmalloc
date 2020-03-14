#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "malign.h"
#include "mmalloc.h"
#include "mtesting.h"


void report_unaligned(size_t size, size_t location, size_t alignment, bool aligned) {
    if (!aligned) {
        printf(
            "ERROR: allocated size %ld at location 0x%lx unaligned on 0x%lx\n",
            size, location, alignment
        );
    }
}


int main() {
    int num_unaligned = 0;

    int min_size = 1 << 4;
    int max_size = 10 << 20;

    long seed_val = set_rand48_seed();
    printf("Using seed value: %ld\n", seed_val);

    for (int i = 0; i < 10; i++) {
        size_t size = get_random_int(min_size, max_size);

        void *ptr = mmalloc(size);
        assert(ptr != NULL);

        size_t location = (size_t) ptr;

        bool aligned_8 = is_aligned(location, 8);
        bool aligned_16 = is_aligned(location, 16);

        report_unaligned(size, location, 8, aligned_8);
        report_unaligned(size, location, 16, aligned_16);

        if ((!aligned_8) || (!aligned_16)) {
            num_unaligned++;
        }
    }

    if (num_unaligned) {
        return 1;
    }

    return 0;
}