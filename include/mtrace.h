#pragma once

#include <stddef.h>


enum Function { F_MALLOC, F_CALLOC, F_REALLOC, F_FREE };

void print_trace(int func_id, size_t size);