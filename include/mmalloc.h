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

block_t *get_list_head(block_t **plist_head);
block_t *get_list_tail(block_t **plist_head);
void append_to_list(block_t **plist_head, block_t *block);
int remove_from_list(block_t **plist_head, block_t *block);

#define GET_USED_LIST_HEAD() get_list_head(get_used_list_ptr())
#define GET_USED_LIST_TAIL() get_list_tail(get_used_list_ptr())
#define APPEND_TO_USED_LIST(block) append_to_list(get_used_list_ptr(), block)
#define REMOVE_FROM_USED_LIST(block) remove_from_list(get_used_list_ptr(), block)