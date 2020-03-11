#include <string.h>
#include <unistd.h>

#include "mtrace.h"


void print_trace(int func_id, size_t size) {
    // Low level output of the allocation size. Have to use `write` since:
    // - printf uses malloc
    // - even putchar interferes with some programs

    // 29 chars + \0
    char buf[30] = "                            \n";
    char ascii_code_zero = 48;

    switch (func_id) {
        case F_MALLOC:
            memcpy(buf, "malloc:", 7);
            break;
        case F_CALLOC:
            memcpy(buf, "calloc:", 7);
            break;
        case F_REALLOC:
            memcpy(buf, "realloc:", 8);
            break;
        case F_FREE:
            memcpy(buf, "free:", 5);
            break;
    }

    if (size > 0) {
        int pos = sizeof(buf) - 3;
        while (size >= 10) {
            char digit = size % 10;
            buf[pos--] = ascii_code_zero + digit;
            size = size / 10;
        }
        buf[pos] = ascii_code_zero + size;
    }

    // write to stderr
    write(2, buf, sizeof(buf));
}
