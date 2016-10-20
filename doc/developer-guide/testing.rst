Testing
=======

To ensure high code quility each module is tested extensively by many
test suites. The test suites are executed both on native Linux and on
many of the supported boards. See `Test suites`_ for a list of all
test suites that are executed before each release.

The native Linux test suites are executed automatically on each commit.

Test result: https://travis-ci.org/eerimoq/simba

Code coverage: https://codecov.io/gh/eerimoq/simba

Hardware setup
--------------

Below is a picture of all supported boards connected to a USB hub. The
USB hub is connected to a linux PC (not in the picture) that executes
test suites on all boards.

.. figure:: ../images/boards-test-setup.jpg
   :target: ../_images/boards-test-setup.jpg

   The boards are (from left to right): :doc:`../boards/arduino_nano`,
   :doc:`../boards/arduino_mega`, :doc:`../boards/arduino_due`,
   :doc:`../boards/stm32vldiscovery`, :doc:`../boards/esp12e` and
   :doc:`../boards/esp01`

A short description of the setup:

- The DS3231 device (on the breadboard to the left) is connected over
  i2c to the :doc:`../boards/arduino_mega`.

- CAN0 is connected to CAN1 on the :doc:`../boards/arduino_due`. The
  CAN driver is tested by sending frames between the two CAN devices.

- The UART of the :doc:`../boards/stm32vldiscovery` board is connected
  to a serial to USB adaptor. DTR on the adaptor is used to reset the
  board.

- The :doc:`../boards/esp12e` also has a serial to USB adaptor
  connected. RTS is used to set the board in flashing mode (GPIO0) and
  DTR is used to reset the board (REST).

Test suites
-----------

Below is a list of all test suites that are executed before every
release. They are listed per board.

.. include:: testing-suites.rst
