#include <assert.h>

#include "mmalloc.h"
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

    // Before appending there are no used blocks

    block = GET_USED_LIST_HEAD();
    assert(block == NULL);

    block = GET_USED_LIST_TAIL();
    assert(block == NULL);

    // After appending the first block both the head and tail are the first block

    APPEND_TO_USED_LIST(fst);

    block = GET_USED_LIST_HEAD();
    assert(block == fst);

    block = GET_USED_LIST_TAIL();
    assert(block == fst);

    // After appending the second block the head and tail are the first and second

    APPEND_TO_USED_LIST(snd);

    block = GET_USED_LIST_HEAD();
    assert(block == fst);

    block = GET_USED_LIST_TAIL();
    assert(block == snd);

    // After appending the third block the head and tail are the first and third

    APPEND_TO_USED_LIST(thd);

    block = GET_USED_LIST_HEAD();
    assert(block == fst);

    block = GET_USED_LIST_TAIL();
    assert(block == thd);

    // We can follow the links between the blocks

    assert(fst->next_block->size == snd->size);
    assert(snd->next_block->size == thd->size);
    assert(thd->next_block == NULL);

    return 0;
}