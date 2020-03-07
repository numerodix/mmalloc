#include <assert.h>
#include <unistd.h>

#include "mmalloc.h"


block_t *USED_BLOCKS = NULL;
block_t *FREE_BLOCKS = NULL;


// Operations on a single block

block_t *init_block(void *ptr, size_t size, block_t *next_block) {
    block_t *block = (block_t *) ptr;

    block->size = size;
    block->next_block = next_block;

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

void append_to_list(block_t **plist_head, block_t *block) {
    block_t *last = get_list_tail(plist_head);

    if (last) {
        last->next_block = block;
    } else {
        *plist_head = block;
    }
}

int remove_from_list(block_t **plist_head, block_t *block) {
    block_t *current = get_list_head(plist_head);

    if (!current) {
        return -1;
    }

    if (current == block) {
        *plist_head = current->next_block;
        current->next_block = NULL;
        return 0;
    }

    block_t *previous;
    while (current->next_block) {
        previous = current;
        current = current->next_block;

        if (current == block) {
            previous->next_block = current->next_block;
            current->next_block = NULL;
            return 0;
        }
    }

    return -1;
}

block_t *pop_from_list(block_t **plist_head, size_t min_size) {
    block_t *current = get_list_head(plist_head);

    if (!current) {
        return NULL;
    }

    if (current->size >= min_size) {
        *plist_head = current->next_block;
        current->next_block = NULL;
        return current;
    }

    block_t *previous;
    while (current->next_block) {
        previous = current;
        current = current->next_block;

        if (current->size >= min_size) {
            previous->next_block = current->next_block;
            current->next_block = NULL;
            return current;
        }
    }

    return NULL;
}


void *mmalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    // look up the current break
    void *ptr_current = sbrk(0);
    assert((size_t) ptr_current != -1);

    // compute new break
    size_t needed_size = size + sizeof(block_t);
    void *ptr_new = ptr_current + needed_size;

    // set the new break
    int res = brk(ptr_new);
    assert(res == 0);

    // create a block and add it to the used list
    block_t *block = init_block(ptr_current, size, NULL);
    APPEND_TO_USED_LIST(block);

    // return a data pointer
    return get_block_data_pointer(block);
}


void mfree(void *ptr) {
    if (!ptr) {
        return;
    }

    // used the pointer to find the block
    block_t *block = as_block_pointer(ptr);

    // remove it from the used list
    int res = REMOVE_FROM_USED_LIST(block);
    assert(res == 0);
}