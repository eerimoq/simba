About
=====

`Simba` is a microkernel and build framework. It aims to make embedded
programming easy and portable.

Source code repository: http://github.com/eerimoq/simba

Design goals
------------

* Rapid development.
* Clean interfaces.
* Small memory footprint.
* Portability.

Microkernel features
--------------------

* Threads, scheduled by a simple priority based scheduler.
* Channels, for inter-thread communication.
* Timers.
* Counting semaphores.
* Drivers (spi, uart, ...)
* Debug shell.

Framework features
------------------

* Package handler (using Python `pip`).

Board support
-------------

* Ardunio Nano
* Arduino Pro Mini (same as Arduino Nano)
* Arduino Due
