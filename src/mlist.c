#include <unistd.h>

#include "malign.h"
#include "mlist.h"


// Pointers to the lists

block_t *USED_BLOCKS = NULL;
block_t *FREE_BLOCKS = NULL;

#define NUM_FREE_LISTS (MAX_SIZE_INDEX - MIN_SIZE_INDEX)

block_t *FREE_BLOCKS_LIST_POINTERS[MAX_SIZE_INDEX + 1] = {NULL};


// Operations on a single block

block_t *init_block(void *ptr, size_t size_index) {
    block_t *block = (block_t *) ptr;

    block->prev_block = NULL;
    block->next_block = NULL;
    block->size_index = size_index;

    return block;
}


void *get_block_data_pointer(block_t* block) {
    return block + sizeof(block_t);
}


block_t *as_block_pointer(void *ptr) {
    block_t *block = (block_t *) ptr;
    return block - sizeof(block_t);
}


// List operations


block_t **get_free_list_ptr() {
    return &FREE_BLOCKS;
}


block_t **get_used_list_ptr() {
    return &USED_BLOCKS;
}


block_t *get_list_head(block_t **plist_head) {
    return *plist_head;
}


block_t *get_list_tail(block_t **plist_head) {
    block_t *current = get_list_head(plist_head);

    if (!current) {
        return NULL;
    }

    while (current->next_block) {
        current = current->next_block;
    }

    return current;
}


void prepend_to_list(block_t **plist_head, block_t *block) {
    block_t *prev_head = *plist_head;

    if (!prev_head) {
        *plist_head = block;
        return;
    }

    prev_head->prev_block = block;
    block->next_block = prev_head;
    *plist_head = block;
}


int remove_from_list(block_t **plist_head, block_t *block) {
    block_t *current_head = get_list_head(plist_head);

    // if `block` is in the list then it has at least:
    // - a next_block, or
    // - a prev_block, or
    // - it's equal to `current_head`
    if ((!block->prev_block) && (!block->next_block) && (block != current_head)) {
        return -1;
    }

    block_t *before = block->prev_block;
    block_t *after = block->next_block;

    block->prev_block = NULL;
    block->next_block = NULL;

    if (before) {
        before->next_block = after;
    }
    if (after) {
        after->prev_block = before;
    }

    if (block == current_head) {
        *plist_head = after;
    }

    return 0;
}


block_t *pop_list_head(block_t **plist_head) {
    block_t *current_head = *plist_head;

    if (!current_head) {
        return NULL;
    }

    block_t *after = current_head->next_block;

    current_head->next_block = NULL;

    if (after) {
        after->prev_block = NULL;
    }
    *plist_head = after;

    return current_head;
}


void prepend_to_a_free_list(block_t *block) {
    block_t **plist_head = &FREE_BLOCKS_LIST_POINTERS[block->size_index];
    prepend_to_list(plist_head, block);
}


block_t *pop_from_a_free_list(size_t size_index) {
    block_t **plist_head = &FREE_BLOCKS_LIST_POINTERS[size_index];
    return pop_list_head(plist_head);
}
