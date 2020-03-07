#include <assert.h>

#include "mmalloc.h"


int main() {
    // We need two things here:
    // - a target block to use as next_block
    // - some memory on the stack to overwrite (we will also use a block for that)

    block_t target;
    target.size = 3;

    block_t dummy;

    // use the dummy memory to construct a block on top of it, pointing to target
    block_t *block = init_block(&dummy, 1, &target);

    // check that it was constructed correctly
    assert(block->size == 1);
    assert(block->next_block->size == target.size);

    // check that the block data pointer points just past the block header
    void *dptr = get_block_data_pointer(block);
    assert(dptr == block + sizeof(block_t));

    // check that we can get back to a block pointer from the data pointer
    block_t *block2 = as_block_pointer(dptr);
    assert(block2 == block);

    return 0;
}