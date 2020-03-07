#pragma once

#include <stddef.h>


void *mmalloc(size_t size);
void *mcalloc(size_t nmemb, size_t size);
void *mrealloc(void *ptr, size_t size);
void mfree(void *ptr);


#ifdef EXPORT_REAL_API
void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
#endif