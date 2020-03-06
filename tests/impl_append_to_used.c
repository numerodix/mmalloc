#include <assert.h>

#include "mmalloc.h"


int main() {
    // We need some dummy stack allocated memory
    block_t dummy1;
    block_t dummy2;
    block_t dummy3;

    // The variable we'll use for lookups
    block_t *block;

    block_t *fst = init_block(&dummy1, 27, NULL);
    block_t *snd = init_block(&dummy2, 33, NULL);
    block_t *thd = init_block(&dummy3, 48, NULL);

    // Before appending there are no used blocks

    block = get_first_used_block();
    assert(block == NULL);

    block = get_last_used_block();
    assert(block == NULL);

    // After appending the first block both the head and tail are the first block

    append_to_used_blocks(fst);

    block = get_first_used_block();
    assert(block == fst);

    block = get_last_used_block();
    assert(block == fst);

    // After appending the second block the head and tail are the first and second

    append_to_used_blocks(snd);

    block = get_first_used_block();
    assert(block == fst);

    block = get_last_used_block();
    assert(block == snd);

    // After appending the third block the head and tail are the first and third

    append_to_used_blocks(thd);

    block = get_first_used_block();
    assert(block == fst);

    block = get_last_used_block();
    assert(block == thd);

    // We can follow the links between the blocks

    assert(fst->next_block->sz == snd->sz);
    assert(snd->next_block->sz == thd->sz);
    assert(thd->next_block == NULL);

    return 0;
}