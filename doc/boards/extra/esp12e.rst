ESP-12 pinout
-------------

.. image:: ../images/boards/esp12e-pinout.png
   :width: 75%
   :target: ../_images/esp12e-pinout.png

Flashing
--------

1. Connect 3.3 V to VCC and ground to GND.
2. Attach the flash jumper (to the right in the picture).
3. Turn on the power.
4. Upload the software to Flash using esptool.
5. The application starts automatically when the download is completed.

Hardware
--------

- 3.3 V power supply and logical level voltage.
- Boot message at 76800 buad on a virgin board. Blue, red and RGB LEDs
  turned on.
- 4 MB Flash.

How to determine the Flash size:

.. code-block:: text

   $ python esptool.py --port /dev/ttyUSB0 flash_id 
   Connecting...
   head: 0 ;total: 0
   erase size : 0
   Manufacturer: e0
   Device: 4016

Device 4016 gives a Flash of size ``2 ^ (16 - 1) / 8 = 4096 kB = 4 MB``.
