:mod:`circular_heap` --- Circular heap
======================================

.. module:: circular_heap
   :synopsis: Circular heap.

The circular heap is a dynamic memory allocator allocating buffers in
a circular buffer. This puts a restriction on the user to free
allocated buffers in the same order as they were allocated. This
allocator is useful if you know the allocation order and need a low
memory overhead on each allocated buffer.

----------------------------------------------

Source code: :github-blob:`src/kernel/kernel/circular_heap.h`

Test code: :github-blob:`tst/kernel/circular_heap/main.c`

----------------------------------------------

.. doxygenfile:: kernel/circular_heap.h
   :project: simba
