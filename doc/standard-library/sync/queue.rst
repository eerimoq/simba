:mod:`queue` --- Queue channel
==============================

.. module:: queue
   :synopsis: Queue channel.

The most common channel is the queue. It can be either synchronous or
semi-asynchronous. In the synchronous version the writing thread will
block until all written data has been read by the reader. In the
semi-asynchronous version the writer writes to a buffer within the
queue, and only blocks all data does not fit in the buffer. The buffer
size is selected by the application when initializing the queue.

----------------------------------------------

Source code: :github-blob:`src/sync/queue.h`

Test code: :github-blob:`tst/sync/queue/main.c`

Test coverage: :codecov:`src/sync/queue.c`

----------------------------------------------

.. doxygenfile:: sync/queue.h
   :project: simba
