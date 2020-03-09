#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "mtesting.h"


int set_rand48_seed() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    long seed_val = ts.tv_sec | ts.tv_nsec;

    srand48(seed_val);

    return seed_val;
}


int get_random_int(int min_value, int max_value) {
    assert(min_value <= max_value);

    int value = drand48() * max_value;

    if (value < min_value) {
        value += min_value;
    }

    return value;
}