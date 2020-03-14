#include <assert.h>

#include "malign.h"


int main() {
    // 32 byte is the smallest
    assert(get_standard_size(8) == 32);
    assert(get_standard_size(16) == 32);
    assert(get_standard_size(24) == 32);
    assert(get_standard_size(32) == 32);

    // round up to next power of 2
    assert(get_standard_size(33) == 64);
    assert(get_standard_size(333) == 512);
    assert(get_standard_size(1025) == 2048);
    assert(get_standard_size(22460) == 0x8000);
    assert(get_standard_size(527423) == 0x100000);

    return 0;
}