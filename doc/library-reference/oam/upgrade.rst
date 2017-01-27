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
          http://192.168.0.7/oam/upgrade/application/write
   > curl http://192.168.0.7/oam/upgrade/application/is_valid
   yes

Then start it using HTTP.

.. code-block:: text

   > curl http://192.168.0.7/kernel/sys/reboot
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
   $ kernel/sys/reboot
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
   $ oam/upgrade/application/load_kermit   # Type '\+c' to return to kermit.
   C-Kermit> send application.ubin

Then start it using the serial port.

.. code-block:: text

   C-Kermit> connect
   $ oam/upgrade/application/is_valid
   yes
   $ kernel/sys/reboot
   Welcome to the test application!

Bootloader
----------

Four protocols are available to upload an application to the board;
HTTP, TFTP, Kermit and UDS.

Upgrade binary file (.ubin)
^^^^^^^^^^^^^^^^^^^^^^^^^^^

This is the format of a .ubin file. All fields except `data` are part
of the header.

.. code-block:: text

   SIZE       TYPE  DESCRIPTION
      4   uint32_t  header version
      4   uint32_t  header size in bytes
      4   uint32_t  data size in bytes
     20  uint8_t[]  SHA1 of the data
     1+   c-string  data description
      4   uint32_t  CRC32 of the header (not including this field)
     0+  uint8_t[]  data

File system commands
^^^^^^^^^^^^^^^^^^^^

These file system commands are available to perform the application
software upgrade.

.. code-block:: text

   /oam/upgrade/application/erase
   /oam/upgrade/application/kermit/load
   /oam/upgrade/application/is_valid

HTTP requests
^^^^^^^^^^^^^

These HTTP requests are available to perform the application software
upgrade.

.. code-block:: text

   GET /oam/upgrade/application/erase
   POST /oam/upgrade/application/write
   GET /oam/upgrade/application/is_valid

Application erase
%%%%%%%%%%%%%%%%%

Request:

.. code-block:: text

    GET /oam/upgrade/application/erase HTTP/1.1
    Host: 192.168.0.7
    User-Agent: curl/7.47.0
    Accept: */*

Successful response:

.. code-block:: text

   HTTP/1.1 200 OK
   Content-Type: text/plain
   Content-Length: 16

   erase successful

Error response:

.. code-block:: text

  HTTP/1.1 400 Bad Request
  Content-Type: text/plain
  Content-Length: 16

  erase failed

Application write
%%%%%%%%%%%%%%%%%

Request:

.. code-block:: text

  POST /oam/upgrade/application/write HTTP/1.1
  Host: 192.168.0.7
  User-Agent: curl/7.47.0
  Accept: */*
  Content-Type: application/octet-stream
  Content-Length: 537072
  Expect: 100-continue

  <upgrade binary file (.ubin)>

Successful response:

.. code-block:: text

  HTTP/1.1 200 OK
  Content-Type: text/plain
  Content-Length: 16

  write successful

Error response:

.. code-block:: text

  HTTP/1.1 400 Bad Request
  Content-Type: text/plain
  Content-Length: 16

  write failed

Application is valid
%%%%%%%%%%%%%%%%%%%%

Request:

.. code-block:: text

   GET /oam/upgrade/application/is_valid HTTP/1.1
   Host: 192.168.0.7
   User-Agent: curl/7.47.0
   Accept: */*

Response:

.. code-block:: text

   HTTP/1.1 200 OK
   Content-Type: text/plain
   Content-Length: 40

   {yes, no}

TFTP file transfer
^^^^^^^^^^^^^^^^^^

Only write, aka "put", in binary mode is supported.

Application
-----------

File system commands
^^^^^^^^^^^^^^^^^^^^

This file system command is available in the application.

.. code-block:: text

   /oam/upgrade/bootloader/enter

HTTP requests
^^^^^^^^^^^^^

This HTTP request is available in the application; given that the
application starts a HTTP server with it registered.

.. code-block:: text

   GET /oam/upgrade/bootloader/enter

----------------------------------------------

Source code: :github-blob:`src/oam/upgrade.h`,
:github-blob:`src/oam/upgrade.c`,
:github-tree:`src/oam/upgrade/application`,
:github-tree:`src/oam/upgrade/bootloader`

Test code: :github-blob:`tst/oam/upgrade/kermit/main.c`,
:github-blob:`tst/oam/upgrade/uds/main.c`

Test coverage: :codecov:`src/oam/upgrade.c`,
:codecov-tree:`src/oam/upgrade/application`,
:codecov-tree:`src/oam/upgrade/bootloader`

Example code: :github-blob:`examples/upgrade/bootloader/main.c`,
:github-blob:`examples/upgrade/application/main.c`

----------------------------------------------

.. doxygenfile:: oam/upgrade.h
   :project: simba
