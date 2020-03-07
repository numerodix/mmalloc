#pragma once

#include <stddef.h>


void *mmalloc(size_t size);

void mfree(void *ptr);



// A block is stored at a position just before the start of every slice 
// of memory that malloc returns.
// Successive allocations return blocks that participate in a linked list.
struct _block {
    size_t size;
    struct _block* next_block;
};
typedef struct _block block_t;


// Constructs a block at location `ptr`.
block_t *init_block(void *ptr, size_t size, block_t *next_block);

void *get_block_data_pointer(block_t* block);
block_t *as_block_pointer(void *ptr);


// Returns a pointer to head pointer to the free list
block_t **get_free_list_ptr();

// Returns a pointer to head pointer to the used list
block_t **get_used_list_ptr();

// Returns a pointer to the block at the head of the list
block_t *get_list_head(block_t **plist_head);

// Returns a pointer to the block at the tail of the list
block_t *get_list_tail(block_t **plist_head);

// Appends the block to the list
void append_to_list(block_t **plist_head, block_t *block);

// Removes the block from the list
int remove_from_list(block_t **plist_head, block_t *block);

// Finds the first block in the list whose size is at least `min_size`,
// then removes it and returns it
block_t *pop_from_list(block_t **plist_head, size_t min_size);


#define GET_USED_LIST_HEAD() get_list_head(get_used_list_ptr())
#define GET_USED_LIST_TAIL() get_list_tail(get_used_list_ptr())
#define APPEND_TO_USED_LIST(block) append_to_list(get_used_list_ptr(), block)
#define REMOVE_FROM_USED_LIST(block) remove_from_list(get_used_list_ptr(), block)

#define GET_FREE_LIST_HEAD() get_list_head(get_free_list_ptr())
#define GET_FREE_LIST_TAIL() get_list_tail(get_free_list_ptr())
#define APPEND_TO_FREE_LIST(block) append_to_list(get_free_list_ptr(), block)
#define POP_FROM_FREE_LIST(min_size) pop_from_list(get_free_list_ptr(), min_size)
