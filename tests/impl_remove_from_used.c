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

    append_to_used_blocks(fst);

    res = remove_from_used_blocks(fst);
    assert(res == 0);

    block = get_first_used_block();
    assert(block == NULL);

    // Append the first and second - remove the second

    append_to_used_blocks(fst);
    append_to_used_blocks(snd);

    res = remove_from_used_blocks(snd);
    assert(res == 0);

    block = get_first_used_block();
    assert(block == fst);

    res = remove_from_used_blocks(fst);
    assert(res == 0);

    block = get_first_used_block();
    assert(block == NULL);

    // Append the first and second - remove the first

    append_to_used_blocks(fst);
    append_to_used_blocks(snd);

    res = remove_from_used_blocks(fst);
    assert(res == 0);

    block = get_first_used_block();
    assert(block == snd);

    res = remove_from_used_blocks(snd);
    assert(res == 0);

    block = get_first_used_block();
    assert(block == NULL);

    // Append all three - remove the second

    append_to_used_blocks(fst);
    append_to_used_blocks(snd);
    append_to_used_blocks(thd);

    res = remove_from_used_blocks(snd);
    assert(res == 0);

    block = get_first_used_block();
    assert(block == fst);

    block = get_last_used_block();
    assert(block == thd);

    assert(fst->next_block->sz == thd->sz);

    return 0;
}
