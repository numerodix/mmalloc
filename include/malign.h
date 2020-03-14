#pragma once

#include <stdbool.h>
#include <unistd.h>


#define MIN_SIZE_INDEX 5
#define MAX_SIZE_INDEX 63


// Aligns `location` on an `alignment` boundary
size_t align_location(size_t location, size_t alignment);

// Returns true if `location` is aligned on `alignment`, otherwise return false
bool is_aligned(size_t location, size_t alignment);


// Returns the base 2 exponent (position of the most significant bit) of `size`
size_t get_base2_exponent(size_t size);

// Returns the size index (range capped exponent) given `exponent`
size_t get_size_index(size_t exponent);

// Returns 2^size_index
size_t get_power_of_2(size_t size_index);