Socket devices
==============

The Linux socket device implementation allows another program to
simulate the hardware.

Protocol
--------

Devices
~~~~~~~

These drivers suppors the socket device protocol.

Uart
^^^^

The UART socket is equivalent to a serial port, it streams data to and
from the application.

Pin
^^^

Sends ``high`` or ``low`` when written to given device.

Pwm
^^^

Sends ``frequency = <value>`` and ``duty_cycle = <value>`` when set on
given device.

Device setup
~~~~~~~~~~~~

Defined messages:

.. code-block:: text

   Device request message:
                
   +---------+---------+----------------+
   | 4b type | 4b size | <size>b device |
   +---------+---------+----------------+

   `device` is the device name as a string. No NULL termination is
   required.
   
   Device response message:
                
   +---------+---------+-----------+
   | 4b type | 4b size | 4b result |
   +---------+---------+-----------+

   `result` is zero(0) on success, and otherwise a negative error
   code.

   TYPE  SIZE  DESCRIPTION
   --------------------------------------
      1     n  Uart device request.
      2     4  Uart device response.
      3     n  Pin device request.
      4     4  Pin device response.
      5     n  Pwm device request.
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
