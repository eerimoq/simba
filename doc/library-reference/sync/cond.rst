:mod:`cond` --- Condition variable
==================================

.. module:: cond
   :synopsis: Condition variable.

A condition variable is a synchronization primitive used to unblock
zero, one or all thread(s) waiting for the condition variable.

Example usage
-------------

This is a small example of signalling a waiting thread.

.. code-block:: c

   struct cond_t cond;
   struct mutex_t mutex;
   int resource = 0;

   /* Initialize the condition variable and a mutex. */
   cond_init(&cond);
   mutex_init(&mutex);

   /* This thread waits for a signal from the signaller thread. */
   void *waiter_main()
   {
       mutex_lock(&mutex);
       cond_wait(&cond, &mutex, NULL)
       mutex_unlock(&mutex);
   }

   /* This thread signals the waiter thread. */
   void *signaller_main()
   {
       /* The mutex is optional when signalling a condition. */
       mutex_lock(&mutex);
       cond_signal(&cond)
       mutex_unlock(&mutex);
   }

----------------------------------------------

Source code: :github-blob:`src/sync/cond.h`, :github-blob:`src/sync/cond.c`

Test code: :github-blob:`tst/sync/cond/main.c`

Test coverage: :codecov:`src/sync/cond.c`

----------------------------------------------

.. doxygenfile:: sync/cond.h
   :project: simba
