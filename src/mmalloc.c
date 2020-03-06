#include <assert.h>
#include <unistd.h>

#include "mmalloc.h"


block_t *USED_BLOCKS = NULL;
block_t *FREE_BLOCKS = NULL;


block_t *init_block(void *ptr, size_t size, block_t *next_block) {
    // Make sure we have enough space for the block header!
    assert(size >= sizeof(block_t));

    block_t *block = (block_t *) ptr;

    block->sz = size;
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

block_t *get_first_used_block() {
    return USED_BLOCKS;
}

block_t *get_last_used_block() {
    block_t *current = get_first_used_block();

    if (!current) {
        return NULL;
    }

    while (current->next_block) {
        current = current->next_block;
    }

    return current;
}

void append_to_used_blocks(block_t *block) {
    block_t *last = get_last_used_block();

    if (last) {
        last->next_block = block;
    } else {
        USED_BLOCKS = block;
    }
}

int remove_from_used_blocks(block_t *block) {
    block_t *current = get_first_used_block();

    if (!current) {
        return -1;
    }

    if (current == block) {
        USED_BLOCKS = current->next_block;
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


void *mmalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    // look up the current break
    void *ptr_current = sbrk(0);
    assert((size_t) ptr_current != -1);

    // compute new break
    void *ptr_new = ptr_current + size;

    // set the new break
    int res = brk(ptr_new);
    assert(res == 0);

    return ptr_current;
}


void mfree(void *ptr) {

}