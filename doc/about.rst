About
=====

`Simba` is an RTOS and build framework. It aims to make embedded
programming easy and portable.

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

* Threads scheduled by a priority based cooperative or preemptive
  scheduler.
* Channels for inter-thread communication.
* Timers.
* Counting semaphores.
* Device drivers (spi, uart, ...)
* A simple shell.
* Internet protocols (TCP, UDP, HTTP, ...).
* ...

Framework features
------------------

* Package handler (using Pythons' `pip`).
* ...

Board support
-------------

.. include:: about/about.rst
