:mod:`sem` --- Counting semaphores
==================================

.. module:: sem
   :synopsis: Counting semaphores.

The semaphore is a synchronization primitive used to protect a shared
resource. A semaphore counts the number of threds holding the
semaphore, and suspends threads when the maximum number of threads
holds the semaphore. When a resource becomes available, a suspended
thread is resumed.

A semaphore initialized with count one(1) is called a binary
semaphore. A binary semaphore can only be taken by one thread at a
time.

----------------------------------------------

Source code: :github-blob:`src/kernel/kernel/sem.h`

Test code: :github-blob:`tst/kernel/sem/main.c`

----------------------------------------------

.. doxygenfile:: kernel/sem.h
   :project: simba
