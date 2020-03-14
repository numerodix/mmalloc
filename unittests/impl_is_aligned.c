#include <assert.h>
#include <stdio.h>

#include "malign.h"


size_t cases[][3] = {
    // 2 byte alignment
    {0, 2, true},
    {1, 2, false},
    {2, 2, true},
    {3, 2, false},
    {4, 2, true},
    {5, 2, false},

    // 4 byte alignment
    {0, 4, true},
    {1, 4, false},
    {2, 4, false},
    {3, 4, false},
    {4, 4, true},
    {5, 4, false},
    {6, 4, false},
    {7, 4, false},
    {8, 4, true},
    {9, 4, false},

    // 8 byte alignment
    {0, 8, true},
    {1, 8, false},
    {2, 8, false},
    {3, 8, false},
    {4, 8, false},
    {5, 8, false},
    {6, 8, false},
    {7, 8, false},
    {8, 8, true},
    {9, 8, false},
    {10, 8, false},
    {11, 8, false},
    {12, 8, false},
    {13, 8, false},
    {14, 8, false},
    {15, 8, false},
    {16, 8, true},
    {17, 8, false},

    // 16 byte alignment
    {15, 16, false},
    {16, 16, true},
    {17, 16, false},
    {31, 16, false},
    {32, 16, true},
    {33, 16, false},
    {63, 16, false},
    {64, 16, true},
    {65, 16, false},

    // 8 byte alignment
    {0x56090b94b410, 8, true},
    {0x56090b94b411, 8, false},
    {0x56090b94b412, 8, false},
    {0x56090b94b413, 8, false},
    {0x56090b94b414, 8, false},
    {0x56090b94b415, 8, false},
    {0x56090b94b416, 8, false},
    {0x56090b94b417, 8, false},
    {0x56090b94b418, 8, true},
    {0x56090b94b419, 8, false},
    {0x56090b94b41a, 8, false},
    {0x56090b94b41b, 8, false},
    {0x56090b94b41c, 8, false},
    {0x56090b94b41d, 8, false},
    {0x56090b94b41e, 8, false},
    {0x56090b94b41f, 8, false},
    {0x56090b94b420, 8, true},

    // 16 byte alignment
    {0x56090b94b410, 16, true},
    {0x56090b94b411, 16, false},
    {0x56090b94b412, 16, false},
    {0x56090b94b413, 16, false},
    {0x56090b94b414, 16, false},
    {0x56090b94b415, 16, false},
    {0x56090b94b416, 16, false},
    {0x56090b94b417, 16, false},
    {0x56090b94b418, 16, false},
    {0x56090b94b419, 16, false},
    {0x56090b94b41a, 16, false},
    {0x56090b94b41b, 16, false},
    {0x56090b94b41c, 16, false},
    {0x56090b94b41d, 16, false},
    {0x56090b94b41e, 16, false},
    {0x56090b94b41f, 16, false},
    {0x56090b94b420, 16, true},

    // sentinel
    {-1, -1, false},
};


void report_incorrect(size_t location, size_t alignment, bool expected, bool actual) {
    if (expected && (!actual)) {
        printf(
            "ERROR: location 0x%lx IS aligned on 0x%lx, but is_aligned said it's not\n",
            location, alignment
        );
    } else if ((!expected) && actual) {
        printf(
            "ERROR: location 0x%lx IS NOT aligned on 0x%lx, but is_aligned said it is\n",
            location, alignment
        );
    }
}


int main() {
    int i = 0;
    int incorrect = 0;

    while (1) {
        size_t location = cases[i][0];
        size_t alignment = cases[i][1];
        bool expected = cases[i][2];

        // we've reached the sentinel
        if (location == (size_t) -1) {
            break;
        }

        bool actual = is_aligned(location, alignment);

        // print the incorrect result
        report_incorrect(location, alignment, expected, actual);

        // keep track of how many incorrect answers we have
        if (actual != expected) {
            incorrect++;
        }

        i++;
    }

    if (incorrect) {
        return 1;
    }

    return 0;
}