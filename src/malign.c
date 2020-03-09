#include <limits.h>

#include "malign.h"


size_t align_location(size_t location, size_t alignment) {
    size_t mask = ULONG_MAX ^ (alignment - 1);
    return location & mask;
}


bool is_aligned(size_t location, size_t alignment) {
    return (location & (alignment - 1)) == 0;
}