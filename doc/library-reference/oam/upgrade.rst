:mod:`upgrade` --- Software upgrade
===================================

.. module:: upgrade
   :synopsis: Software upgrade.

The flash memory is partitioned into two partitions; the bootloader
partition and the application partition. The software in the
bootloader partition can perform a software upgrade of the application
partition by using the erase and write commands.

.. warning:: The WiFi connection is often lost during the erase
             operation. Unknown why.

Bootloader
----------

Four protocols are available to upload an application to the board;
HTTP, TFTP, Kermit and UDS.

File system commands
^^^^^^^^^^^^^^^^^^^^

These file system commands are available to perform the application
software upgrade.

.. code-block:: text

   /oam/upgrade/application/erase
   /oam/upgrade/application/kermit/load
   /oam/upgrade/application/sha1

HTTP requests
^^^^^^^^^^^^^

These HTTP requests are available to perform the application software
upgrade.

.. code-block:: text

   GET /oam/upgrade/application/erase
   POST /oam/upgrade/application/write
   GET /oam/upgrade/application/sha1

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

  <application binary data>

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

Application SHA1 hash
%%%%%%%%%%%%%%%%%%%%%

Request:

.. code-block:: text

   GET /oam/upgrade/application/sha1 HTTP/1.1
   Host: 192.168.0.7
   User-Agent: curl/7.47.0
   Accept: */*

Response:

.. code-block:: text

   HTTP/1.1 200 OK
   Content-Type: text/plain
   Content-Length: 40

   ba59caac5f5a80fc52c507d8a47f322a380aa9a1

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

Examples
--------

Here are a few examples of how to upgrade the application using the
different supported protocols.

HTTP
^^^^

Build and upload the bootloader to the board. Build the test
application and use curl to upload it to the Nano32. Then start it!

.. code-block:: text

   > make -C bootloader -s BOARD=nano32 run
   > make -C application -s BOARD=nano32
   > cd application/build/nano32
   > curl --header "Content-Type: application/octet-stream" \
          --data-binary @application.bin \
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
   > cd application/build/nano32
   > tftp 192.168.0.7
   tftp> mode binary
   tftp> put application.bin
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
