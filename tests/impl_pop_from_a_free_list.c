#include <assert.h>

#include "mlist.h"
#include "malign.h"


int main() {
    // We need some dummy stack allocated memory
    block_t dummy1;
    block_t dummy2;
    block_t dummy3;

    // The variable we'll use for lookups
    block_t *block;

    block_t *five = init_block(&dummy1, 5);
    block_t *seven = init_block(&dummy2, 7);
    block_t *sixty_three = init_block(&dummy3, 63);

    // cannot pop from an empty list
    block = pop_from_a_free_list(five->size_index);
    assert(block == NULL);

    // Prepend a block each to different lists

    prepend_to_a_free_list(five);
    prepend_to_a_free_list(seven);
    prepend_to_a_free_list(sixty_three);

    // Pop one block off again

    block_t *five2 = pop_from_a_free_list(five->size_index);
    assert(five == five2);

    // cannot pop from an empty list
    block = pop_from_a_free_list(five->size_index);
    assert(block == NULL);

    // Pop off the others

    block_t *seven2 = pop_from_a_free_list(seven->size_index);
    assert(seven == seven2);

    block_t *sixty_three2 = pop_from_a_free_list(sixty_three->size_index);
    assert(sixty_three == sixty_three2);

    return 0;
}
