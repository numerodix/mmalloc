A simple malloc implementation that uses two singly linked lists:
- A used list that stores all blocks currently in use.
- A free list that stores blocks previously allocated and then free'd.

When an allocation request comes in, we first try to satisfy it using a block
from the free list. If no block large enough can be found, we increase the size
of the heap to create a new block.

No blocks from the free list are ever released back to the OS. The heap only
grows, never shrinks.

Allocations are aligned at 16 byte boundaries.

A mutex is used to ensure exclusive access to shared data structures.

Most obvious shortcomings:
- Blocks are sized according to the size requested instead of using more
  reusable powers-of-2 sizes.
- When picking a block from the free list the first block big enough is chosen,
  with no effort to find one that is optimal size for the request.

The implementation is functional enough to run both single threaded and multi
threaded programs with it, eg. ls, ps, cat, hostname, uname, javac:

```
$ ./runprogram ls
```

The code was developed in a test driven style. Run the tests with:

```
$ ./test
```

Portability:

* Assumes 64bit system.
* Assumes `size_t` is an unsigned long (64bit).
