#include <assert.h>

#include "mlist.h"


int main() {
    // We need some memory on the stack to overwrite
    // (we will also use a block for that)
    block_t dummy;

    // use the dummy memory to construct a block on top of it
    block_t *block = init_block(&dummy, 5);

    // check that it was constructed correctly
    assert(block->prev_block == NULL);
    assert(block->next_block == NULL);
    assert(block->size_index == 5);

    // check that the block data pointer points just past the block header
    void *dptr = get_block_data_pointer(block);
    assert(dptr == block + sizeof(block_t));

    // check that we can get back to a block pointer from the data pointer
    block_t *block2 = as_block_pointer(dptr);
    assert(block2 == block);

    return 0;
}