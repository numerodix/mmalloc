A simple malloc implementation that uses two singly linked lists:
- A used list that stores all blocks currently in use.
- A free list that stores blocks previously allocated and then free'd.

When an allocation request comes in, we first try to satisfy it using a block
from the free list. If no block large enough can be found, we increase the size
of the heap to create a new block.

No blocks from the free list are ever released back to the OS. The heap only
grows, never shrinks.

Most obvious shortcomings:
- Allocations are not aligned.
- Not thread safe.
