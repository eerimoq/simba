:mod:`gnss` --- Global Navigation Satellite System
==================================================

.. module:: gnss
   :synopsis: Global Navigation Satellite System.

`GNSS`_ is a set of global coverage satellite system, including GPS,
GLONASS and Galileo.

This driver reads `NMEA 0183`_ sentences from an input channel (often
a UART driver), parses them and stores position, time and speed in the
driver object.

This driver should be compatible with all GNSS devices sending and
receiving NMEA sentences over a serial port.

Devices known to work with this driver:

-

--------------------------------------------------

Source code: :github-blob:`src/drivers/sensors/gnss.h`, :github-blob:`src/drivers/sensors/gnss.c`

Test code: :github-blob:`tst/drivers/software/gnss/main.c`

Example code: :github-blob:`examples/gnss/main.c`

--------------------------------------------------

.. doxygenfile:: drivers/sensors/gnss.h
   :project: simba

.. _GNSS: https://en.wikipedia.org/wiki/Satellite_navigation
.. _NMEA 0183: https://en.wikipedia.org/wiki/NMEA_0183
