#pragma once

#include <stddef.h>


void *mmalloc(size_t size);

void mfree(void *ptr);



struct _block {
    size_t sz;
    struct _block* next_block;
};
typedef struct _block block_t;


// Constructs a block at location `ptr`.
block_t *init_block(void *ptr, size_t size, block_t *next_block);
void *get_block_data_pointer(block_t* block);
block_t *get_first_used_block();
block_t *get_last_used_block();
void append_to_used_blocks(block_t *block);

block_t *get_first_used_block();