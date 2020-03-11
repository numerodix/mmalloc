#include <assert.h>

#include "mlist.h"


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

    // Before prepending there are no used blocks

    block = GET_USED_LIST_HEAD();
    assert(block == NULL);

    block = GET_USED_LIST_TAIL();
    assert(block == NULL);

    // After prepending the first block both the head and tail are the first block

    PREPEND_TO_USED_LIST(fst);

    block = GET_USED_LIST_HEAD();
    assert(block == fst);

    block = GET_USED_LIST_TAIL();
    assert(block == fst);

    // After prepending the second block the head and tail are the second and first

    PREPEND_TO_USED_LIST(snd);

    block = GET_USED_LIST_HEAD();
    assert(block == snd);

    block = GET_USED_LIST_TAIL();
    assert(block == fst);

    // After prepending the third block the head and tail are the third and first

    PREPEND_TO_USED_LIST(thd);

    block = GET_USED_LIST_HEAD();
    assert(block == thd);

    block = GET_USED_LIST_TAIL();
    assert(block == fst);

    // We can follow the links between the blocks

    assert(thd->next_block->size == snd->size);
    assert(snd->next_block->size == fst->size);
    assert(fst->next_block == NULL);

    return 0;
}
