#include <assert.h>

#include "malign.h"


int main() {
    size_t aligned;

    // 8 byte alignment
    aligned = align_location(0x56090b94b410, 8);
    assert(aligned == 0x56090b94b410);
    assert(true == is_aligned(aligned, 8));

    aligned = align_location(0x56090b94b415, 8);
    assert(aligned == 0x56090b94b410);
    assert(true == is_aligned(aligned, 8));

    aligned = align_location(0x56090b94b418, 8);
    assert(aligned == 0x56090b94b418);
    assert(true == is_aligned(aligned, 8));

    aligned = align_location(0x56090b94b41d, 8);
    assert(aligned == 0x56090b94b418);
    assert(true == is_aligned(aligned, 8));

    // 16 byte alignment
    aligned = align_location(0x56090b94b410, 16);
    assert(aligned == 0x56090b94b410);
    assert(true == is_aligned(aligned, 16));

    aligned = align_location(0x56090b94b418, 16);
    assert(aligned == 0x56090b94b410);
    assert(true == is_aligned(aligned, 16));

    aligned = align_location(0x56090b94b41d, 16);
    assert(aligned == 0x56090b94b410);
    assert(true == is_aligned(aligned, 16));
}