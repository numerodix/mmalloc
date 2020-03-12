#include <assert.h>

#include "mlist.h"


int main() {
    // We need some dummy stack allocated memory
    block_t dummy1;
    block_t dummy2;
    block_t dummy3;

    // The variables we'll use for testing
    block_t *block;

    block_t *fst = init_block(&dummy1, 48);
    block_t *snd = init_block(&dummy2, 33);
    block_t *thd = init_block(&dummy3, 25);

    // Prepend the first block and pop it again

    PREPEND_TO_FREE_LIST(fst);

    block = POP_FROM_FREE_LIST(50);  // too big
    assert(block == NULL);

    block = POP_FROM_FREE_LIST(46);
    assert(block == fst);

    block = POP_FROM_FREE_LIST(46);  // list was empty
    assert(block == NULL);

    block = GET_FREE_LIST_HEAD();
    assert(block == NULL);

    // Prepend the first and second - pop the second

    PREPEND_TO_FREE_LIST(fst);
    PREPEND_TO_FREE_LIST(snd);

    block = POP_FROM_FREE_LIST(28);
    assert(block == snd);

    assert(snd->prev_block == NULL);
    assert(snd->next_block == NULL);

    block = GET_FREE_LIST_HEAD();
    assert(block == fst);

    assert(fst->prev_block == NULL);
    assert(fst->next_block == NULL);

    block = POP_FROM_FREE_LIST(0);
    assert(block == fst);

    block = GET_FREE_LIST_HEAD();
    assert(block == NULL);

    // Prepend the first and second - pop the second

    PREPEND_TO_FREE_LIST(fst);
    PREPEND_TO_FREE_LIST(snd);

    block = POP_FROM_FREE_LIST(35);
    assert(block == fst);

    assert(fst->prev_block == NULL);
    assert(fst->next_block == NULL);

    block = GET_FREE_LIST_HEAD();
    assert(block == snd);

    assert(snd->prev_block == NULL);
    assert(snd->next_block == NULL);

    block = POP_FROM_FREE_LIST(0);
    assert(block == snd);

    block = GET_FREE_LIST_HEAD();
    assert(block == NULL);

    // Prepend all three - remove the second

    PREPEND_TO_FREE_LIST(fst);
    PREPEND_TO_FREE_LIST(snd);
    PREPEND_TO_FREE_LIST(thd);

    block = POP_FROM_FREE_LIST(30);
    assert(block == snd);

    assert(snd->prev_block == NULL);
    assert(snd->next_block == NULL);

    block = GET_FREE_LIST_HEAD();
    assert(block == thd);

    block = GET_FREE_LIST_TAIL();
    assert(block == fst);

    assert(thd->prev_block == NULL);

    assert(thd->next_block->size == fst->size);
    assert(fst->prev_block->size == thd->size);

    assert(fst->next_block == NULL);

    return 0;
}
