Bootloader
==========

A main purpose if a bootloader is to load and start the application.

Two implementation are available; UDS and Console.

UDS
---

Unified Diagnostics Services is a protocol often used in the
Automotive industry to upload software to a MCU.

.. code-block:: text

   make -C app/uds BAORD=arduino_due release run

Console
-------

A text based bootloader for humans.

.. code-block:: text

   make -C app/console BAORD=arduino_due release run

Test application
================

The test application can be uploaded to the board using the
bootloaders. Its linker script relocates it to the correct address in
the Flash memory.

.. code-block:: text

   make -C app/testapp BAORD=arduino_due release

Example
=======

Build and upload the Console bootloader. Build the test application
and use Kermit to upload it to the Arduino Due. Then start it!

.. code-block:: text

   make -s -C app/console BOARD=arduino_due clean release run
   make -s -C app/testapp BOARD=arduino_due clean release
   kermit
   C-Kermit>connect
   $ app/load_kermit       # Type '\+c' to return to kermit.
   C-Kermit> send app/testapp/test_application.bin
   C-Kermit> connect
   $ app/start
   starting the application
   Welcome to the test application!
