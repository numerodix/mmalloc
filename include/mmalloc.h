#pragma once

#include <stddef.h>


void *mmalloc(size_t size);

void mfree(void *ptr);