About
=====

`Simba` is an operating system and build framework. It aims to make
embedded programming easy and portable.

Project homepage: http://github.com/eerimoq/simba

Design goals
------------

* Rapid development.
* Clean interfaces.
* Small memory footprint.
* No dynamic memory allocation.
* Portability.

OS features
-----------

* Threads scheduled by a priority based cooperative scheduler.
* Channels for inter-thread communication.
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

* :doc:`Arduino Due <boards/arduino_due>`
* :doc:`Arduino Mega <boards/arduino_mega>`
* :doc:`Ardunio Nano <boards/arduino_nano>`
* :doc:`Cygwin <boards/cygwin>`
* :doc:`Linux <boards/linux>`
