:mod:`sem` --- Counting semaphores
==================================

.. module:: sem
   :synopsis: Counting semaphores.

The semaphore is a synchronization primitive used to protect a shared
resource. A semaphore counts the number of resources taken, and
suspends threads when the maximum number of resources are taken. When
a resource becomes available, a suspended thread is resumed.

A semaphore initialized with `count_max` one(1) is called a binary
semaphore. A binary semaphore can only be taken by one thread at a
time and can be used to signal that an event has occured. That is,
`sem_give()` may be called multiple times and the semaphore resource
count will remain at zero(0) until `sem_take()` is called.

Example usage
-------------

This is a small example of protecting a shared resource with a
semaphore.

.. code-block:: c

   struct sem_t sem;
   int resource = 0;

   /* Initialize the semaphore. */
   sem_init(&sem, 0, 1);

   /* Increment the shared resource by one. */
   sem_take(&sem, NULL);
   resource++;
   sem_give(&sem, 1);

----------------------------------------------

Source code: :github-blob:`src/sync/sem.h`, :github-blob:`src/sync/sem.c`

Test code: :github-blob:`tst/sync/sem/main.c`

Test coverage: :codecov:`src/sync/sem.c`

----------------------------------------------

.. doxygenfile:: sync/sem.h
   :project: simba
