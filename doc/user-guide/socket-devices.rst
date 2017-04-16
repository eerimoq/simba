Socket devices
==============

The Linux socket device drivers implementation allows an external
program to simulate the hardware. The external program communicates
with the Simba application using TCP sockets, one socket for each
device.

The Python script ``bin/socket_device.py`` can be used to monitor and
send data to a device.

Here is an example of how to build a linux application with the
Arduino Mega pinout ...

.. code-block:: text

   $ make BOARD=linux PINOUT=arduino_mega run

... and then monitor digital pin 0, ``d0``.

.. code-block:: text

   > socket_device.py pin d0
   Connecting to localhost:47000... done.
   Requesting pin device d0... done.
   $
   pin(d0) RX: high
   pin(d0) RX: high
   pin(d0) RX: low

Alternatively, monitor all devices at the same time with the monitor
make target.

.. code-block:: text

   $ make BOARD=linux PINOUT=arduino_mega monitor
   Connecting to localhost:47000... done.
   Requesting uart device 0... done.
   ...
   Connecting to localhost:47000... done.
   Requesting pin device 0... done.
   Connecting to localhost:47000... done.
   Requesting pin device 1... done.
   ...
   $
   pin(0) RX: low
   uart(0) RX: b'\n'
   pin(1) RX: high

Protocol
--------

At startup the Simba application creates a socket and starts listening
for clients on port 47000.

Devices
~~~~~~~

These drivers supports the socket device protocol at the moment. More
to be added when needed.

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

Can
^^^

Sends and receives frames on the format ``<id>,<extended>,<data
size>,<data>``. ``<id>`` and ``<data>`` are hexadecimal numbers not
prefixed with ``0x``. ``<extended>`` and ``<size>`` are decimal
integers.

.. code-block:: text

   > socket_device.py can 0
   Connecting to localhost:47000... done.
   Requesting can device 0... done.
   $ 00000005,1,2,0011000000000000<Enter>
   can(0) TX: 00000005,1,2,0011000000000000
   can(0) RX: 00000006,1,2,0112000000000000

Device request message
~~~~~~~~~~~~~~~~~~~~~~

This message is sent to the Simba application to request a device.

.. code-block:: text

   +---------+---------+----------------+
   | 4b type | 4b size | <size>b device |
   +---------+---------+----------------+

   `device` is the device name as a string without NULL termination.

   TYPE  SIZE  DESCRIPTION
   --------------------------------------
      1     n  Uart device request.
      3     n  Pin device request.
      5     n  Pwm device request.
      7     n  Can device request.

Device response message
~~~~~~~~~~~~~~~~~~~~~~~

This message is the response to the request message.

.. code-block:: text

   +---------+---------+-----------+
   | 4b type | 4b size | 4b result |
   +---------+---------+-----------+

   `result` is zero(0) on success, and otherwise a negative error
   code.

   Defined error codes are:

      ENODEV(19): No device found matching requested device name.

      EADDRINUSE(98): The requested device is already requested and in
                      use.

   TYPE  SIZE  DESCRIPTION
   --------------------------------------
      2     4  Uart device response.
      4     4  Pin device response.
      6     4  Pwm device response.
      8     4  Can device response.
