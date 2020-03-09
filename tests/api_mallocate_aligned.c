#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "malign.h"
#include "mmalloc.h"


size_t sizes[] = {
    2,
    1,
    125,
    11,
    4097,
    34,
    20000,
    13,
    15,
    14,

    -1,   // sentinel
};


void report_unaligned(size_t size, size_t location, size_t alignment, bool aligned) {
    if (!aligned) {
        printf(
            "ERROR: allocated size %ld at location 0x%lx unaligned on 0x%lx\n",
            size, location, alignment
        );
    }
}


int main() {
    int i = 0;
    int num_unaligned = 0;

    while (1) {
        size_t size = sizes[i];

        // we've reached the sentinel
        if (size == -1) {
            break;
        }

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

        i++;
    }

    if (num_unaligned) {
        return 1;
    }

    return 0;
}