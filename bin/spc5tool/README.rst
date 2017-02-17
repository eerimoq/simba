SPC5 Tool
=========

Erase, read and write SPC5 flash memories at a high UART baudrate.

Upload this application to SPC5 MCU using the BAM (Boot Assist
Module).

Protocol
--------

On command success the type field is copied from the request to the
response packet. On failure the response type is set to -1.

This is the packet format:

.. code-block:: text

   +---------+---------+-----------------+--------+
   | 1b type | 2b size | <size>b payload | 2b crc |
   +---------+---------+-----------------+--------+

   TYPE  REQ-SIZE  RSP-SIZE  DESCRIPTION
   ------------------------------------------------
     -1         -         0  Command failure.
      1         0         0  Ping.
      2         8         0  Erase flash.
      3         8         n  Read from flash.
      4       8+n         0  Write to flash.

Command failure
^^^^^^^^^^^^^^^

.. code-block:: text

   +----+---+-----+
   | -1 | 0 | crc |
   +----+---+-----+

Ping
^^^^

.. code-block:: text

   +---+---+-----+
   | 1 | 0 | crc |
   +---+---+-----+

Erase flash
^^^^^^^^^^^

.. code-block:: text

   +---+---+------------+---------+-----+
   | 2 | 8 | 4b address | 4b size | crc |
   +---+---+------------+---------+-----+

Read flash
^^^^^^^^^^

.. code-block:: text

   +---+---+------------+---------+-----+
   | 3 | 8 | 4b address | 4b size | crc |
   +---+---+------------+---------+-----+

Write flash
^^^^^^^^^^^

.. code-block:: text

   +---+----------+------------+---------+--------------+-----+
   | 4 | 8 + size | 4b address | 4b size | <size>b data | crc |
   +---+----------+------------+---------+--------------+-----+
