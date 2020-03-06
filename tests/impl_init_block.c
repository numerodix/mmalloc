#include <assert.h>

#include "mmalloc.h"


int main() {
    // We need two things here:
    // - a target block to use as next_block
    // - some memory on the stack to overwrite (we will also use a block for that)

    block_t target;
    block_t dummy;

    // use the dummy memory to construct a block on top of it, pointing to target
    block_t *block = init_block(&dummy, 27, &target);

    // check that it was constructed correctly
    assert(block->sz == 27);
    assert(block->next_block->sz == target.sz);

    // check that the block data pointer points just past the block header
    void *dptr = get_block_data_pointer(block);
    assert(dptr == block + sizeof(block_t));

    return 0;
}