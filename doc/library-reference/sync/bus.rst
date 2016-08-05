:mod:`bus` --- Message bus
==========================

.. module:: bus
   :synopsis: Message bus.

A message bus provides a software-bus abstraction that gathers all the
communications between a group of threads over a single shared virtual
channel. Messages are transferred on the bus from a sender to one or
more attached listeners. The concept is analogous to the bus concept
found in computer hardware architecture.

Example
-------

In this example there is a bus with three listeners attached;
listerner `0`, `1` and `2`. Listener `0` and `1` are attached to the
bus listening for message id `7`, and listener `2` for message id `9`.

Any thread can write a message to the bus by calling
``bus_write()``. If a message with id `7` is written to the bus, both
listerner `0` and `1` will receive the message. Listener `2` will
receive messages with id `9`.

Messages are read from the listener channel by the thread that owns
the listener.

.. code-block:: text

      +--------------+              +--------------+
      |  listener 0  |              |  listener 2  |
      | id:7, chan:0 |              | id:9, chan:2 |
      +-------+------+              +-------+------+
              |                             |
 BUS  ========+==============+==============+=======
                             |
                     +-------+------+
                     |  listener 1  |
                     | id:7, chan:1 |
                     +--------------+

----------------------------------------------

Source code: :github-blob:`src/sync/bus.h`, :github-blob:`src/sync/bus.c`

Test code: :github-blob:`tst/sync/bus/main.c`

Test coverage: :codecov:`src/sync/bus.c`

----------------------------------------------

.. doxygenfile:: sync/bus.h
   :project: simba
