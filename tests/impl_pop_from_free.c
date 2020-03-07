#include <assert.h>

#include "mmalloc.h"
#include "mlist.h"


int main() {
    // We need some dummy stack allocated memory
    block_t dummy1;
    block_t dummy2;
    block_t dummy3;

    // The variables we'll use for testing
    block_t *block;

    block_t *fst = init_block(&dummy1, 27, NULL);
    block_t *snd = init_block(&dummy2, 33, NULL);
    block_t *thd = init_block(&dummy3, 48, NULL);

    // Append the first block and pop it again

    APPEND_TO_FREE_LIST(fst);

    block = POP_FROM_FREE_LIST(28);  // too big
    assert(block == NULL);

    block = POP_FROM_FREE_LIST(26);
    assert(block == fst);

    block = GET_FREE_LIST_HEAD();
    assert(block == NULL);

    // Append the first and second - pop the second

    APPEND_TO_FREE_LIST(fst);
    APPEND_TO_FREE_LIST(snd);

    block = POP_FROM_FREE_LIST(28);
    assert(block == snd);

    block = GET_FREE_LIST_HEAD();
    assert(block == fst);

    block = POP_FROM_FREE_LIST(0);
    assert(block == fst);

    block = GET_FREE_LIST_HEAD();
    assert(block == NULL);

    // Append the first and second - pop the first

    APPEND_TO_FREE_LIST(fst);
    APPEND_TO_FREE_LIST(snd);

    block = POP_FROM_FREE_LIST(25);
    assert(block == fst);

    block = GET_FREE_LIST_HEAD();
    assert(block == snd);

    block = POP_FROM_FREE_LIST(0);
    assert(block == snd);

    block = GET_FREE_LIST_HEAD();
    assert(block == NULL);

    // Append all three - remove the second

    APPEND_TO_FREE_LIST(fst);
    APPEND_TO_FREE_LIST(snd);
    APPEND_TO_FREE_LIST(thd);

    block = POP_FROM_FREE_LIST(30);
    assert(block == snd);

    block = GET_FREE_LIST_HEAD();
    assert(block == fst);

    block = GET_FREE_LIST_TAIL();
    assert(block == thd);

    assert(fst->next_block->size == thd->size);

    return 0;
}
