#include <assert.h>

#include "mmalloc.h"


int main() {
    // We need some dummy stack allocated memory
    block_t dummy1;
    block_t dummy2;
    block_t dummy3;

    // The variables we'll use for testing
    block_t *block;
    int res;

    block_t *fst = init_block(&dummy1, 27, NULL);
    block_t *snd = init_block(&dummy2, 33, NULL);
    block_t *thd = init_block(&dummy3, 48, NULL);

    // Append the first block and remove it again

    APPEND_TO_USED_BLOCKS(fst);

    res = REMOVE_FROM_USED_BLOCKS(fst);
    assert(res == 0);

    block = GET_FIRST_USED_BLOCK();
    assert(block == NULL);

    // Append the first and second - remove the second

    APPEND_TO_USED_BLOCKS(fst);
    APPEND_TO_USED_BLOCKS(snd);

    res = REMOVE_FROM_USED_BLOCKS(snd);
    assert(res == 0);

    block = GET_FIRST_USED_BLOCK();
    assert(block == fst);

    res = REMOVE_FROM_USED_BLOCKS(fst);
    assert(res == 0);

    block = GET_FIRST_USED_BLOCK();
    assert(block == NULL);

    // Append the first and second - remove the first

    APPEND_TO_USED_BLOCKS(fst);
    APPEND_TO_USED_BLOCKS(snd);

    res = REMOVE_FROM_USED_BLOCKS(fst);
    assert(res == 0);

    block = GET_FIRST_USED_BLOCK();
    assert(block == snd);

    res = REMOVE_FROM_USED_BLOCKS(snd);
    assert(res == 0);

    block = GET_FIRST_USED_BLOCK();
    assert(block == NULL);

    // Append all three - remove the second

    APPEND_TO_USED_BLOCKS(fst);
    APPEND_TO_USED_BLOCKS(snd);
    APPEND_TO_USED_BLOCKS(thd);

    res = REMOVE_FROM_USED_BLOCKS(snd);
    assert(res == 0);

    block = GET_FIRST_USED_BLOCK();
    assert(block == fst);

    block = GET_LAST_USED_BLOCK();
    assert(block == thd);

    assert(fst->next_block->sz == thd->sz);

    return 0;
}
