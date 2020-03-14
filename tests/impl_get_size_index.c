#include <assert.h>

#include "malign.h"


int main() {
    assert(get_size_index(1) == 5);
    assert(get_size_index(4) == 5);
    assert(get_size_index(5) == 5);

    assert(get_size_index(6) == 6);
    assert(get_size_index(7) == 7);

    assert(get_size_index(31) == 31);
    assert(get_size_index(32) == 32);
    assert(get_size_index(33) == 33);

    assert(get_size_index(62) == 62);
    assert(get_size_index(63) == 63);
    assert(get_size_index(64) == 63);
    assert(get_size_index(65) == 63);

    return 0;
}