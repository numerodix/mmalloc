#pragma once

#include <stdbool.h>
#include <unistd.h>


// Aligns `location` on an `alignment` boundary
size_t align_location(size_t location, size_t alignment);

// Returns true if `location` is aligned on `alignment`, otherwise return false
bool is_aligned(size_t location, size_t alignment);

// Returns the next power of 2 up from `num`
size_t next_power_of_2(size_t num);

// Returns a standard size large enough to contain `num`
size_t get_standard_size(size_t num);

// Returns the base 2 exponent (position of the most significant bit) of `size`
size_t get_base2_exponent(size_t size);