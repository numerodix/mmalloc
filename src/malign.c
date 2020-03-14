#include <limits.h>
#include <stdint.h>

#include "malign.h"


#define MIN_STD_SIZE 1 << 5

#define MIN_SIZE_INDEX 5
#define MAX_SIZE_INDEX 63


size_t align_location(size_t location, size_t alignment) {
    size_t mask = ULONG_MAX ^ (alignment - 1);
    return location & mask;
}


bool is_aligned(size_t location, size_t alignment) {
    return (location & (alignment - 1)) == 0;
}


size_t next_power_of_2(size_t num) {
    // num must be 64bit

    num--;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    num |= num >> 32;
    num++;

    return num;
}


size_t get_standard_size(size_t num) {
    if (num < MIN_STD_SIZE) {
        return MIN_STD_SIZE;
    }

    return next_power_of_2(num);
}


size_t get_base2_exponent(size_t size) {
    int counter = 0;

    while (size >>= 1) {
        counter++;
    }

    return counter;
}


size_t get_size_index(size_t exponent) {
    if (exponent < MIN_SIZE_INDEX) {
        return MIN_SIZE_INDEX;
    }

    if (exponent > MAX_SIZE_INDEX) {
        return MAX_SIZE_INDEX;
    }

    return exponent;
}


size_t get_power_of_2(size_t size_index) {
    return 1UL << size_index;
}