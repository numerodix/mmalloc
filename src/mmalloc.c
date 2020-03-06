#include <assert.h>
#include <unistd.h>

#include "mmalloc.h"


block_t *USED_BLOCKS = NULL;
block_t *FREE_BLOCKS = NULL;


block_t *init_block(void *ptr, size_t size, block_t *next_block) {
    block_t *block = (block_t *) ptr;

    block->sz = size;
    block->next_block = next_block;

    return block;
}

size_t get_block_size() {
    return sizeof(block_t);
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
    size_t needed_size = size + get_block_size();
    void *ptr_new = ptr_current + needed_size;

    // set the new break
    int res = brk(ptr_new);
    assert(res == 0);

    // block housekeeping
    block_t *block = init_block(ptr_current, size, NULL);
    append_to_used_blocks(block);
    return get_block_data_pointer(block);
}


void mfree(void *ptr) {
    if (!ptr) {
        return;
    }

    block_t *block = as_block_pointer(ptr);

    int res = remove_from_used_blocks(block);
    assert(res == 0);
}