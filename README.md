# Adventures in writing a malloc

A halfway competent malloc implementation that uses doubly linked lists:

- One used list that stores all blocks currently in use.
- N free lists, each storing unused blocks of the same size, 2^N bytes
  large.

When an allocation request comes in, we first try to satisfy it using a block
from the free list that stores blocks of a matching size. If no block can be
found, we increase the size of the heap to create a new block.

No blocks from the free lists are ever released back to the OS. The heap only
grows, never shrinks.

Features:
- Allocations are aligned at 16 byte boundaries.
- Blocks are sized by rounding up the size requested to the next power of 2,
  in order to make the blocks more reusable.
- A mutex is used to ensure exclusive access to shared data structures.
- All list operations are O(1).

The implementation is functional enough and fast enough to run most programs,
both single threaded and multi threaded, both command line programs and gui
programs.

Examples: ls, ps, cat, hostname, uname, gcc, javac, firefox, vim, kate:

```
$ ./runprogram ls -l
```

The code was developed in a test driven style. Run the tests with:

```
$ ./unittest
```

Portability:

* Assumes 64bit system.
* Assumes `size_t` is an unsigned long (64bit).


## So you want to write a malloc?

When you start out trying to write a malloc implementation the first milestone
is to get it to run on your own toy example programs. Once you've done that
it's time to start testing it against real programs.

You should definitely use the `LD_PRELOAD` trick where you compile your malloc
into a shared library and run existing programs using it. It's almost magical
because you don't have to recompile any of them; you just insert your
malloc/free functions into their symbol search path and they will get called.

```
$ ulimit -c unlimited   # so you can get a core file on a crash
$ LD_PRELOAD=mymalloc.so ls
```

You'll probably want to print from your malloc to see if it's being called or
not, but it's a mistake to use `printf` because that itself calls malloc, so
it's not going to work. You'll need to use a lower level function like `write`.

At this point it's most likely that almost no real programs will run with your
malloc.  The first program I was able to run was `ls` (without arguments), but
not `ls -l` (segfault).  When you get a crash run `gdb <program> core` to see
the stack and this may give you a clue as to what's wrong. 

My first crash was in a function that was calling `strcmp`. If you're lucky you
might see something like this, where the clue "unaligned" is literally part of
the function name:

```
(gdb) bt
#0  __strcmp_sse2_unaligned () at ../sysdeps/x86_64/multiarch/strcmp-sse2-unaligned.S:32
```

One by one you will have to tackle these issues (not necessarily in this order):

1. Make sure the address malloc returns is aligned, because some code expects
   to be able to read a word of memory at a time (8 bytes on a 64bit
   architecture), starting at an aligned address.
2. Make sure the chunk you return is also aligned (ie. padded) at the end, for
   the same reason.
3. Synchronize access to data structures that are used from more than one
   thread. Without this multi threaded programs will crash.
4. Speed, because your malloc is most likely too slow to run interactive
   programs.
5. Efficient memory management. This is the ultimate goal of a malloc
   implementation.
