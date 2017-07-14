:mod:`mutex` --- Mutual exclusion
=================================

.. module:: mutex
   :synopsis: Mutual exclusion.

A mutex is a synchronization primitive used to protect a shared
resource.

Example usage
-------------

This is a small example of protecting a shared resource with a
mutex.

.. code-block:: c

   struct mutex_t mutex;
   int resource = 0;

   /* Initialize the mutex. */
   mutex_init(&mutex);

   /* Increment the shared resource by one. */
   mutex_lock(&mutex);
   resource++;
   mutex_unlock(&mutex);

----------------------------------------------

Source code: :github-blob:`src/sync/mutex.h`, :github-blob:`src/sync/mutex.c`

Test code: :github-blob:`tst/sync/mutex/main.c`

Test coverage: :codecov:`src/sync/mutex.c`

----------------------------------------------

.. doxygenfile:: sync/mutex.h
   :project: simba
