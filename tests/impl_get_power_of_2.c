#include <assert.h>

#include "malign.h"


int main() {
    assert(get_power_of_2(1) == 2);
    assert(get_power_of_2(5) == 32);

    assert(get_power_of_2(63) == 1UL << 63);

    return 0;
}