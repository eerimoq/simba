:mod:`gnss` --- Global Navigation Satellite System
==================================================

.. module:: gnss
   :synopsis: Global Navigation Satellite System.

`GNSS`_ is a set of global coverage satellite system, including GPS,
GLONASS and Galileo.

This drivers parses NMEA sentences from a GNSS device and stores
position, time and speed in RAM.

--------------------------------------------------

Source code: :github-blob:`src/drivers/gnss.h`, :github-blob:`src/drivers/gnss.c`

Test code: :github-blob:`tst/drivers/software/gnss/main.c`

--------------------------------------------------

.. doxygenfile:: drivers/gnss.h
   :project: simba

.. _GNSS: https://en.wikipedia.org/wiki/Satellite_navigation
