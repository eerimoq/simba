:mod:`bus` --- Message bus
==========================

.. module:: bus
   :synopsis: Message bus.

A bus with three listeners attached; listerner 0, 1 and 2.

Any thread can write a message to the bus by calling
``bus_write()``. If a message with id 7 is written to the bus, both
listerner 0 and 1 will receive the message. Listener 2 will receive
messages with id 9.

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

Source code: :source:`src/kernel/kernel/bus.h`

Test code: :source:`tst/kernel/bus/main.c`

----------------------------------------------

.. doxygenfile:: kernel/bus.h
   :project: simba
