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

    // Cannot remove from an empty list

    res = REMOVE_FROM_USED_LIST(fst);
    assert(res == -1);

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

    // cannot remove a second time
    res = REMOVE_FROM_USED_LIST(fst);
    assert(res == -1);

    assert(fst->prev_block == NULL);
    assert(fst->next_block == NULL);

    block = GET_USED_LIST_HEAD();
    assert(block == snd);

    assert(snd->prev_block == NULL);
    assert(snd->next_block == NULL);

    res = REMOVE_FROM_USED_LIST(snd);
    assert(res == 0);

    block = GET_USED_LIST_HEAD();
    assert(block == NULL);

    // Prepend the first and second - remove the second

    PREPEND_TO_USED_LIST(fst);
    PREPEND_TO_USED_LIST(snd);

    res = REMOVE_FROM_USED_LIST(snd);
    assert(res == 0);

    // cannot remove a second time
    res = REMOVE_FROM_USED_LIST(snd);
    assert(res == -1);

    assert(snd->prev_block == NULL);
    assert(snd->next_block == NULL);

    block = GET_USED_LIST_HEAD();
    assert(block == fst);

    assert(fst->prev_block == NULL);
    assert(fst->next_block == NULL);

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

    // cannot remove a second time
    res = REMOVE_FROM_USED_LIST(snd);
    assert(res == -1);

    assert(snd->prev_block == NULL);
    assert(snd->next_block == NULL);

    block = GET_USED_LIST_HEAD();
    assert(block == thd);

    block = GET_USED_LIST_TAIL();
    assert(block == fst);

    assert(thd->prev_block == NULL);

    assert(thd->next_block->size == fst->size);
    assert(fst->prev_block->size == thd->size);

    assert(fst->next_block == NULL);

    return 0;
}
