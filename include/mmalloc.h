#pragma once

#include <stddef.h>


void *mmalloc(size_t size);

void *mcalloc(size_t nmemb, size_t size);

void *mrealloc(void *ptr, size_t size);

void mfree(void *ptr);