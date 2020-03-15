# Ideas

## Notes from reading ptmalloc2 (glibc)

* Blocks in use don't really need to be in a linked list at all. In our case
  that means we don't need a used list. Since removing blocks from the used
  list with O(1) complexity is our only reason for having a doubly linked list
  we don't even need it to be doubly linked. A singly linked list is enough to
  prepend and pop-head in the free lists with O(1) complexity. The only reason
  we would still want to have a used list would be to ascertain that a pointer
  passed to free() really was allocated by us. But that is a programming error
  and a program-abort condition anyway, so it's not that important to detect
  it.

* The user data part of the block is not used when the block is vacant. We
  could actually use it to store the allocator's own metadata in there, like
  pointers to other blocks.

* Instead of calling brk() for every request we could increase the program
  break by the page size (4kb) each time, and just allocate out of that free
  space for as long as we can in the meantime. This reduces the amount of
  system calls.

* All of the allocator's state is kept in a static `malloc_state` struct, which
  contains a mutex, pointers to all the linked lists etc.

* Aligment is: 2 x `size_t`, ie. 16 bytes on a 64bit system.

* Size thresholds: 512b is considered a "large" request. 128kb is considered a
  "very large" request that is considered big enough to allocate using mmap
  instead of using brk().
