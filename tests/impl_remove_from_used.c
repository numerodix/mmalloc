#include <assert.h>

#include "mlist.h"


int main() {
    // We need some dummy stack allocated memory
    block_t dummy1;
    block_t dummy2;
    block_t dummy3;

    // The variables we'll use for testing
    block_t *block;
    int res;

    block_t *fst = init_block(&dummy1, 27);
    block_t *snd = init_block(&dummy2, 33);
    block_t *thd = init_block(&dummy3, 48);

    // Prepend the first block and remove it again

    PREPEND_TO_USED_LIST(fst);

    res = REMOVE_FROM_USED_LIST(fst);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == NULL);

    // Prepend the first and second - remove the first

    PREPEND_TO_USED_LIST(fst);
    PREPEND_TO_USED_LIST(snd);

    res = REMOVE_FROM_USED_LIST(fst);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == snd);

    res = REMOVE_FROM_USED_LIST(snd);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == NULL);

    // Prepend the first and second - remove the second

    PREPEND_TO_USED_LIST(fst);
    PREPEND_TO_USED_LIST(snd);

    res = REMOVE_FROM_USED_LIST(snd);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == fst);

    res = REMOVE_FROM_USED_LIST(fst);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == NULL);

    // Prepend all three - remove the second

    PREPEND_TO_USED_LIST(fst);
    PREPEND_TO_USED_LIST(snd);
    PREPEND_TO_USED_LIST(thd);

    res = REMOVE_FROM_USED_LIST(snd);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == thd);

    block = GET_USED_LIST_TAIL();
    assert(block == fst);

    assert(thd->next_block->size == fst->size);

    return 0;
}
