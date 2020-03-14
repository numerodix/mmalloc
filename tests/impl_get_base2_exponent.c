#include <assert.h>

#include "malign.h"


int main() {
    assert(get_base2_exponent(1UL << 1) == 1);
    assert(get_base2_exponent(1UL << 2) == 2);
    assert(get_base2_exponent(1UL << 3) == 3);
    assert(get_base2_exponent(1UL << 4) == 4);

    assert(get_base2_exponent(1UL << 7) == 7);
    assert(get_base2_exponent(1UL << 8) == 8);
    assert(get_base2_exponent(1UL << 9) == 9);

    assert(get_base2_exponent(1UL << 31) == 31);
    assert(get_base2_exponent(1UL << 32) == 32);
    assert(get_base2_exponent(1UL << 33) == 33);

    assert(get_base2_exponent(1UL << 51) == 51);

    assert(get_base2_exponent(1UL << 63) == 63);
}