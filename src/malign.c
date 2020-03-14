#include <assert.h>
#include <limits.h>
#include <stdint.h>

#include "malign.h"


size_t align_location(size_t location, size_t alignment) {
    size_t mask = ULONG_MAX ^ (alignment - 1);
    return location & mask;
}


bool is_aligned(size_t location, size_t alignment) {
    return (location & (alignment - 1)) == 0;
}


size_t get_base2_exponent(size_t size) {
    int counter = 0;

    // this loop is unfortunate:
    // for the common small allocations like 32 bytes it's a few loops
    // for larger ones (32k - 1mb) it's 15-20 loops
    while (size >>= 1) {
        counter++;
    }

    return counter;
}


size_t get_size_index(size_t exponent) {
    if (exponent < MIN_SIZE_INDEX) {
        return MIN_SIZE_INDEX;
    }

    assert(exponent <= MAX_SIZE_INDEX);
    return exponent;
}


size_t get_power_of_2(size_t size_index) {
    return 1UL << size_index;
}
