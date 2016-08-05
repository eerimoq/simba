:mod:`circular_heap` --- Circular heap
======================================

.. module:: circular_heap
   :synopsis: Circular heap.

The circular heap is a dynamic memory allocator allocating buffers in
a circular buffer. This puts a restriction on the user to free
allocated buffers in the same order as they were allocated. This
allocator is useful if you know the allocation order and need a low
memory overhead on each allocated buffer and no memory fragmentation.

Below is an example of the internal state of a circular heap when
buffers are allocated and freed.

1. After initialization `begin`, `alloc` and `free` have the same
   value. All memory is available for allocation.

   .. code:: text

      begin
      alloc
       free                                                  end
        |-----------------------------------------------------|

2. Allocating a buffer increments `alloc`.

   .. code:: text

      begin
       free                     alloc                        end
        |=========================|---------------------------|

3. Allocating another buffer increments `alloc` once again.

   .. code:: text

      begin
       free                                      alloc       end
        |==========================================|----------|

4. Freeing the first buffer increments `free` to the position of the
   first `alloc`.

   .. code:: text

      begin                      free            alloc       end
        |-------------------------|================|----------|

5. Allocating a buffer that is bigger than the available space between
   `alloc` and `end` results in a buffer starting at `begin`. The
   memory between the old `alloc` and `end` will be unused.

   .. code:: text

      begin              alloc   free                        end
        |==================|------|================|oooooooooo|

6. Freeing the second buffer increments `free` to the position of the
   second `alloc`.

   .. code:: text

      begin              alloc                    free       end
        |==================|-----------------------|oooooooooo|

7. Freeing the third buffer sets `free` to `alloc`. All memory is
   available for allocation once again.

   .. code:: text

                         alloc
      begin               free                               end
        |------------------|----------------------------------|

8. Done!

----------------------------------------------

Source code: :github-blob:`src/alloc/circular_heap.h`, :github-blob:`src/alloc/circular_heap.c`

Test code: :github-blob:`tst/alloc/circular_heap/main.c`

Test coverage: :codecov:`src/alloc/circular_heap.c`

----------------------------------------------

.. doxygenfile:: alloc/circular_heap.h
   :project: simba
