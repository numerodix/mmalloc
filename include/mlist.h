#pragma once

#include <stddef.h>


// A block is stored at a position just before the start of every slice 
// of memory that malloc returns.
// Successive allocations return blocks that participate in a linked list.
struct _block {
    struct _block* prev_block;
    struct _block* next_block;
    size_t size_index;
    size_t _unused;  // padding
};
typedef struct _block block_t;


// Constructs a block at location `ptr`.
block_t *init_block(void *ptr, size_t size_index);

// Returns a pointer to the data location of a block
void *get_block_data_pointer(block_t* block);

// Given a data location, returns a pointer to its block
block_t *as_block_pointer(void *ptr);


// Returns a pointer to the head pointer of the used list
block_t **get_used_list_ptr();

// Returns a pointer to the block at the head of the list
block_t *get_list_head(block_t **plist_head);

// Returns a pointer to the block at the tail of the list
block_t *get_list_tail(block_t **plist_head);

// Prepends the block to the head of the list
void prepend_to_list(block_t **plist_head, block_t *block);

// Removes the block from the list
int remove_from_list(block_t **plist_head, block_t *block);

// Removes the head of the list and returns it
block_t *pop_list_head(block_t **plist_head);


#define GET_USED_LIST_HEAD() get_list_head(get_used_list_ptr())
#define GET_USED_LIST_TAIL() get_list_tail(get_used_list_ptr())
#define PREPEND_TO_USED_LIST(block) prepend_to_list(get_used_list_ptr(), block)
#define REMOVE_FROM_USED_LIST(block) remove_from_list(get_used_list_ptr(), block)


// Prepends the block to the right free list, based on block->size_index
void prepend_to_a_free_list(block_t *block);

// Using the list identified by `size_index`, removes the head of the list
// and returns it
block_t *pop_from_a_free_list(size_t size_index);
