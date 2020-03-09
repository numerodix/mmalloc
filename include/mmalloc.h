#pragma once

#include <stddef.h>


void *mmalloc(size_t size);
void *mcalloc(size_t nmemb, size_t size);
void *mrealloc(void *ptr, size_t size);
void mfree(void *ptr);


#ifdef EXPORT_REAL_API
__attribute__ ((visibility ("default"))) void *malloc(size_t size);
__attribute__ ((visibility ("default"))) void *calloc(size_t nmemb, size_t size);
__attribute__ ((visibility ("default"))) void *realloc(void *ptr, size_t size);
__attribute__ ((visibility ("default"))) void free(void *ptr);
#endif