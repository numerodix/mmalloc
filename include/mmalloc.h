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
block_t *as_block_pointer(void *ptr);

block_t **get_used_list_ptr();

block_t *get_first_block(block_t **plist_head);
block_t *get_last_block(block_t **plist_head);
void append_to_blocks(block_t **plist_head, block_t *block);
int remove_from_blocks(block_t **plist_head, block_t *block);

#define GET_FIRST_USED_BLOCK() get_first_block(get_used_list_ptr())
#define GET_LAST_USED_BLOCK() get_last_block(get_used_list_ptr())
#define APPEND_TO_USED_BLOCKS(block) append_to_blocks(get_used_list_ptr(), block)
#define REMOVE_FROM_USED_BLOCKS(block) remove_from_blocks(get_used_list_ptr(), block)