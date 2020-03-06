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

    APPEND_TO_USED_LIST(fst);

    res = REMOVE_FROM_USED_LIST(fst);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == NULL);

    // Append the first and second - remove the second

    APPEND_TO_USED_LIST(fst);
    APPEND_TO_USED_LIST(snd);

    res = REMOVE_FROM_USED_LIST(snd);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == fst);

    res = REMOVE_FROM_USED_LIST(fst);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == NULL);

    // Append the first and second - remove the first

    APPEND_TO_USED_LIST(fst);
    APPEND_TO_USED_LIST(snd);

    res = REMOVE_FROM_USED_LIST(fst);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == snd);

    res = REMOVE_FROM_USED_LIST(snd);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == NULL);

    // Append all three - remove the second

    APPEND_TO_USED_LIST(fst);
    APPEND_TO_USED_LIST(snd);
    APPEND_TO_USED_LIST(thd);

    res = REMOVE_FROM_USED_LIST(snd);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == fst);

    block = GET_USED_LIST_TAIL();
    assert(block == thd);

    assert(fst->next_block->sz == thd->sz);

    return 0;
}
