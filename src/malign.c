#include <limits.h>

#include "malign.h"


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