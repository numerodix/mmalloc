A simple malloc implementation that uses two singly linked lists:
- A used list that stores all blocks currently in use.
- A free list that stores blocks previously allocated and then free'd.

When an allocation request comes in, we first try to satisfy it using a block
from the free list. If no block large enough can be found, we increase the size
of the heap to create a new block.

No blocks from the free list are ever released back to the OS. The heap only
grows, never shrinks.

Most obvious shortcomings:
- When picking a block from the free list the first block big enough is chosen,
  with no effort to find one that is optimal size for the request.
- Allocations are not aligned.
- The implementation is not thread safe.

The implementation is functional enough to run some simple programs with it,
eg. ls, ps, cat, hostname, uname:

```
$ ./runprogram uname -a
```

The code was developed in a test driven style. Run the tests with:

```
$ ./test
```
