:mod:`upgrade` --- Software upgrade
===================================

.. module:: upgrade
   :synopsis: Software upgrade.

Upgrade/upload an application over the air (OTA) or using a
cable. HTTP, TFTP, Kermit and UDS protocols are supported.

The flash memory is partitioned into two partitions; the bootloader
partition and the application partition. The software in the
bootloader partition can perform a software upgrade of the application
partition by using the erase and write commands.

.. warning:: The WiFi connection is often lost during the erase
             operation on ESP32. Troubleshooting ongoing...

Debug file system commands
--------------------------

Five debug file system commands are available, all located in the
directory ``oam/upgrade/``.

+-------------------------------+-----------------------------------------------------------------+
|  Command                      | Description                                                     |
+===============================+=================================================================+
|  ``application/enter``        | Enter the appliaction.                                          |
+-------------------------------+-----------------------------------------------------------------+
|  ``application/erase``        | Erase the appliaction. May not be called from the |br|          |
|                               | application about to be erased.                                 |
+-------------------------------+-----------------------------------------------------------------+
|  ``application/is_valid``     | Check is there is a valid application in the memory.            |
+-------------------------------+-----------------------------------------------------------------+
|  ``kermit/upload``            | Upload a upgrade binary file using the Kermit file |br|         |
|                               | transfer protocol.                                              |
+-------------------------------+-----------------------------------------------------------------+
|  ``bootloader/enter``         | Enter the bootloader.                                           |
+-------------------------------+-----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

   $ oam/upgrade/application/is_valid
   yes

HTTP requests
-------------

Five HTTP requests are available. Form the URL by prefixing them with
``http://<hostname>/oam/upgrade/``,
ie. ``http://<hostname>/oam/upgrade/application/is_valid``.

+---------------------------+------+--------------------------------------------------------------+
|  Request                  | Type | Description                                                  |
+===========================+======+==============================================================+
|  ``application/enter``    |  GET | Enter the appliaction.                                       |
+---------------------------+------+--------------------------------------------------------------+
|  ``application/erase``    |  GET | Erase the appliaction. May not be called from the |br|       |
|                           |      | application about to be erased.                              |
+---------------------------+------+--------------------------------------------------------------+
|  ``application/is_valid`` |  GET | Check is there is a valid application in the memory.         |
+---------------------------+------+--------------------------------------------------------------+
|  ``upload``               | POST | Upload a upgrade binary file using the Kermit file |br|      |
|                           |      | transfer protocol.                                           |
+---------------------------+------+--------------------------------------------------------------+
|  ``bootloader/enter``     |  GET | Enter the bootloader.                                        |
+---------------------------+------+--------------------------------------------------------------+

TFTP file transfer
------------------

Only upload, aka "put", in binary mode is supported.

Examples
--------

Here are a few examples of how to upgrade the application using the
different supported protocols.

HTTP
^^^^

Build and upload the bootloader to the board over the serial port.

.. code-block:: text

   > make -C bootloader -s BOARD=nano32 run

Build the test application and use curl to upload it to the Nano32
over HTTP.

.. code-block:: text

   > make -C application -s BOARD=nano32
   > cd application/build/nano32
   > curl http://192.168.0.7/oam/upgrade/application/is_valid
   no
   > curl --header "Content-Type: application/octet-stream" \
          --data-binary @application.ubin \
          http://192.168.0.7/oam/upgrade/upload
   > curl http://192.168.0.7/oam/upgrade/application/is_valid
   yes

Then start it using HTTP.

.. code-block:: text

   > curl http://192.168.0.7/oam/upgrade/application/enter
   Welcome to the test application!

TFTP
^^^^

Build and upload the bootloader to the board over the serial port.

.. code-block:: text

   > make -C bootloader -s BOARD=nano32 run

Build the test application and use tftp to upload it to the Nano32
over TFTP.

.. code-block:: text

   > make -C application -s BOARD=nano32
   > cd application/build/nano32
   > tftp 192.168.0.7
   tftp> mode binary
   tftp> put application.ubin
   5460544 bytes
   tftp> q

Then start it using the serial port.

.. code-block:: text

   > kermit
   C-Kermit>connect
   $ oam/upgrade/application/is_valid
   yes
   $ oam/upgrade/application/enter
   Welcome to the test application!

Kermit
^^^^^^

Build and upload the bootloader to the board over the serial port.

.. code-block:: text

   > make -s -C bootloader BOARD=arduino_due run

Build the test application and use Kermit to upload it to the Arduino
Due over the serial port.

.. code-block:: text

   > make -s -C application BOARD=arduino_due
   > cd application/build/arduino_due
   > kermit
   C-Kermit>connect
   $ oam/upgrade/application/is_valid
   no
   $ oam/upgrade/application/erase
   $ oam/upgrade/kermit/upload       # Type '\+c' to return to kermit.
   C-Kermit> send application.ubin

Then start it using the serial port.

.. code-block:: text

   C-Kermit> connect
   $ oam/upgrade/application/is_valid
   yes
   $ oam/upgrade/application/enter
   Welcome to the test application!

----------------------------------------------

Source code: :github-blob:`src/oam/upgrade.h`,
:github-blob:`src/oam/upgrade.c`, :github-tree:`src/oam/upgrade`

Test code: :github-blob:`tst/oam/upgrade/main.c`,
:github-blob:`tst/oam/upgrade/kermit/main.c`,
:github-blob:`tst/oam/upgrade/uds/main.c`

Test coverage: :codecov:`src/oam/upgrade.c`,
:codecov-tree:`src/oam/upgrade`

Example code: :github-blob:`examples/upgrade/bootloader/main.c`,
:github-blob:`examples/upgrade/application/main.c`

----------------------------------------------

.. doxygenfile:: oam/upgrade.h
   :project: simba

.. |br| raw:: html

   <br />
