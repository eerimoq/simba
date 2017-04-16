Socket devices
==============

The Linux socket device implementation allows another program to
simulate the hardware. The program connunicates with the Simba
application using TCP sockets, one socket for each device.

At startup the Simba application creates a socket and starts listening
for clients on port 47000.

The Python script ``bin/socket_device.py`` can be used to monitor and
send data to a device.

Here is an example of how to monitor digital pin 0, ``d0``:

.. code-block:: text

   $ socket_device.py pin d0
   high
   high
   low

Protocol
--------

Devices
~~~~~~~

These drivers supports the socket device protocol.

Uart
^^^^

The UART socket is equivalent to a serial port, it streams data to and
from the application.

Pin
^^^

Sends ``high`` or ``low`` when written to given device. Input is not
supported yet.

Pwm
^^^

Sends ``frequency = <value>`` and ``duty_cycle = <value>`` when set on
given device.

Device request message
~~~~~~~~~~~~~~~~~~~~~~

This message is sent to the Simba application to request a device.

.. code-block:: text

   +---------+---------+----------------+
   | 4b type | 4b size | <size>b device |
   +---------+---------+----------------+

   `device` is the device name as a string. No NULL termination is
   required.

   TYPE  SIZE  DESCRIPTION
   --------------------------------------
      1     n  Uart device request.
      3     n  Pin device request.
      5     n  Pwm device request.

Device response message
~~~~~~~~~~~~~~~~~~~~~~~

This message is the response to the request message.

.. code-block:: text

   +---------+---------+-----------+
   | 4b type | 4b size | 4b result |
   +---------+---------+-----------+

   `result` is zero(0) on success, and otherwise a negative error
   code.

   TYPE  SIZE  DESCRIPTION
   --------------------------------------
      2     4  Uart device response.
      4     4  Pin device response.
      6     4  Pwm device response.

Build system tips
-----------------

Add the rows below to your application's makefile to include the
target board's pin mapping in the linux build. In the example below
``arduino_mega`` is the target board.

.. code-block:: makefile

   INC += $(SIMBA_ROOT)/src/boards/arduino_mega
   SRC += $(SIMBA_ROOT)/src/boards/arduino_mega/board.c
   SRC_IGNORE += $(SIMBA_ROOT)/src/boards/linux/board.c
