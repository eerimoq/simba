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
* No dynamic memory allocation.
* Portability.

Microkernel features
--------------------

* Threads, scheduled by a simple priority based scheduler.
* Channels, for inter-thread communication.
* Timers.
* Counting semaphores.
* Device drivers (spi, uart, ...)
* A simple shell.
* ...

Framework features
------------------

* Package handler (using Pythons' `pip`).
* ...

Board support
-------------

* Ardunio Nano
* Arduino Pro Mini (same as Arduino Nano)
* Arduino Due
