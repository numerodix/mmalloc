#include <assert.h>

#include "malign.h"


int main() {
    assert(next_power_of_2(0) == 0);
    assert(next_power_of_2(1) == 1);
    assert(next_power_of_2(2) == 2);
    assert(next_power_of_2(3) == 4);
    assert(next_power_of_2(4) == 4);
    assert(next_power_of_2(5) == 8);
    assert(next_power_of_2(31) == 32);
    assert(next_power_of_2(32) == 32);
    assert(next_power_of_2(33) == 64);
    assert(next_power_of_2(0x9ecd32e) == 0x10000000);
    assert(next_power_of_2(0x3ffe18f59bd7e2) == 0x40000000000000);
}