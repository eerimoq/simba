:mod:`upgrade` --- Software upgrade
===================================

.. module:: upgrade
   :synopsis: Software upgrade.

Bootloader
----------

The bootloader is used to update and start the application.

Four protocols are available to upload an application to the board;
HTTP, TFTP, Kermit and UDS.

File system commands
^^^^^^^^^^^^^^^^^^^^

.. code-block:: text

   /oam/upgrade/application/erase
   /oam/upgrade/application/kermit/load
   /oam/upgrade/application/sha1
   /oam/upgrade/bootloader/enter

HTTP requests
^^^^^^^^^^^^^

.. code-block:: text

   GET /oam/upgrade/application/erase
   POST /oam/upgrade/application/write
   GET /oam/upgrade/application/sha1

   GET /oam/upgrade/bootloader/enter

TFTP file transfer
^^^^^^^^^^^^^^^^^^

Only write, aka "put", in binary mode is supported.

Examples
--------

HTTP
^^^^

Build and upload the bootloader to the board. Build the test
application and use curl to upload it to the Nano32. Then start it!

.. code-block:: text

   > make -C bootloader -s BOARD=nano32 run
   > make -C application -s BOARD=nano32
   > curl --header "Content-Type: application/octet-stream" \
          --data-binary @application/build/nano32/application.bin \
          http://192.168.0.7/oam/upgrade/application/write
   > curl http://192.168.0.7/kernel/sys/reboot
   Welcome to the test application!

TFTP
^^^^

Build and upload the bootloader to the board. Build the test
application and use curl to upload it to the Nano32. Then start it!

.. code-block:: text

   > make -C bootloader -s BOARD=nano32 run
   > make -C application -s BOARD=nano32
   > kermit
   C-Kermit>connect
   $ oam/upgrade/application/erase
   $                            # Type '\+q' to exit kermit.
   > tftp 192.168.0.7
   tftp> mode binary
   tftp> put application/build/nano32/application.bin
   5460544 bytes 
   tftp> q
   > kermit
   C-Kermit>connect
   $ kernel/sys/reboot
   Welcome to the test application!

Kermit
^^^^^^

Build and upload the bootloader to the board. Build the test
application and use Kermit to upload it to the Arduino Due. Then start
it!

.. code-block:: text

   > make -s -C bootloader BOARD=arduino_due run
   > make -s -C application BOARD=arduino_due
   > kermit
   C-Kermit>connect
   $ oam/upgrade/application/erase
   $ oam/upgrade/application/load_kermit   # Type '\+c' to return to kermit.
   C-Kermit> send application/build/arduino_due/application.bin
   C-Kermit> connect
   $ kernel/sys/reboot
   Welcome to the test application!

See :doc:`../../user-guide/configuration` for a list of all
configuration variables.

----------------------------------------------

Source code: :github-blob:`src/oam/upgrade.h`, :github-blob:`src/oam/upgrade.c`

Test code: :github-blob:`tst/oam/upgrade/kermit/main.c`,
:github-blob:`tst/oam/upgrade/uds/main.c`

Test coverage: :codecov:`src/oam/upgrade.c`

Example code: :github-blob:`examples/upgrade/bootloader/main.c`,
:github-blob:`examples/upgrade/application/main.c`

----------------------------------------------

.. doxygenfile:: oam/upgrade.h
   :project: simba
