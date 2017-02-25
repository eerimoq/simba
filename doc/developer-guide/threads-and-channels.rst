Threads and channels
====================

A thread is the basic execution entity. A scheduler controls the
execution of threads.

A simple thread that waits for an event from another thread.

.. code-block:: c

    #include "simba.h"

    struct event_t event;

    void *my_thread_main(void *arg_p)
    {
        uint32_t mask;

        while (1) {
            mask = 0x1;
            event_read(&event, &mask, sizeof(mask));

            std_printf(FSTR("Event received!\r\n"));
        }

        return (NULL);
    }

Threads usually communicates over channels. There are many kinds of
channels; queue, socket and event, to mention a few. All three
implementing the same abstract channel interface (see
:github-blob:`src/sync/chan.h`). This abstraction makes channel very
powerful as a synchronization primitive. They can be seen as limited
functionality file descriptors in linux.

The most common channel is the
:doc:`../library-reference/sync/queue`. It can be either synchronous
or semi-asynchronous. In the synchronous version the writing thread
will block until all written data has been read by the reader. In the
semi-asynchronous version the writer writes to a buffer within the
queue, and only blocks all data does not fit in the buffer. The buffer
size is selected by the application.
