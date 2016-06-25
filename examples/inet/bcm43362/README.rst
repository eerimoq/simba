About
=====

This is an example of how to transport IP packets over a serial line
using the SLIP framing protocol.

Setup
=====

.. code:: text

        Linux PC                              Board

   +-----------------+    SLIP/UART 1  +-----------------+
   | ip: 169.254.1.1 o-----------------o ip: 169.254.1.2 |
   +-----------------+                 +-----------------+

Configure the SLIP interface ``sl0`` on the Linux PC and ping the board.

.. code:: text

   slattach -L -s 115200 -p slip /dev/ttyUSB0 &
   ifconfig sl0 169.254.1.1 pointopoint 169.254.1.2 up
   ping 169.254.1.2

If everything works, the board will respond to the ping request.
