#include <assert.h>

#include "malign.h"


int main() {
    int loops = 1 << 24;

    for (int i=0; i<loops; i++) {
        get_base2_exponent(1UL << 5);
        get_base2_exponent(1UL << 10);
        get_base2_exponent(1UL << 15);
        get_base2_exponent(1UL << 20);
        get_base2_exponent(1UL << 25);
        get_base2_exponent(1UL << 30);
    }

    return 0;
}
