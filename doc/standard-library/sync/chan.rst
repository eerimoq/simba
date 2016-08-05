:mod:`chan` --- Abstract channel communication
==============================================

.. module:: chan
   :synopsis: Abstract channel communication.

Threads often communicate over channels. The producer thread or isr
writes data to a channel and the consumer reads it. The may be
multiple producers writing to a single channel, but only one consumer
is allowed.

In the first example, ``thread 0`` and ``thread 1`` communicates over
a channel. ``thread 0`` writes data to the channel and ``thread 1``
read the written data.

.. code-block:: text

      +------------+             +------------+
      |  thread 0  |  channel 0  |  thread 1  |
      |            +=============+            |
      |  producer  |             |  consumer  |
      +------------+             +------------+

In the socond example, ``isr 0`` and ``thread 2`` communicates over a
channel. ``isr 0`` writes data to the channel and ``thread 2`` read
the written data.

.. code-block:: text

      +------------+             +------------+
      |   isr 0    |  channel 1  |  thread 2  |
      |            +=============+            |
      |  producer  |             |  consumer  |
      +------------+             +------------+

----------------------------------------------

Source code: :github-blob:`src/sync/chan.h`, :github-blob:`src/sync/chan.c`

Test coverage: :codecov:`src/sync/chan.c`

----------------------------------------------

.. doxygenfile:: sync/chan.h
   :project: simba
