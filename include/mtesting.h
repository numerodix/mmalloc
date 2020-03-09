#pragma once


// Seeds the rand48 generator using the current system time and
// returns the seed value.
int set_rand48_seed();

// Returns a random integer between `min_value` and `max_value`.
int get_random_int(int min_value, int max_value);