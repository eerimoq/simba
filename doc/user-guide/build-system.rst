Build system
============

Simba build system
------------------

The `Simba` build system is based on `GNU Make`.

Targets
^^^^^^^

+-----------------+----------------------------------------------------------------+
|  Name           |  Description                                                   |
+=================+================================================================+
|  all            |  Compile and link the application.                             |
+-----------------+----------------------------------------------------------------+
|  clean          |  Remove all generated files and folders.                       |
+-----------------+----------------------------------------------------------------+
|  new            |  clean + all                                                   |
+-----------------+----------------------------------------------------------------+
|  upload         |  all + Upload the application to the device.                   |
+-----------------+----------------------------------------------------------------+
| console         |  Open a serial console on /dev/arduino with baudrate BAUDRATE. |
+-----------------+----------------------------------------------------------------+
|  run            |  all + upload + Wait for application output.                   |
+-----------------+----------------------------------------------------------------+
|  run-debugger   |  Run the application in the debugger, break at main.           |
+-----------------+----------------------------------------------------------------+
|  report         |  Print the test report from a previous run.                    |
+-----------------+----------------------------------------------------------------+
|  test           |  run + report                                                  |
+-----------------+----------------------------------------------------------------+
|  release        |  Compile with NASSERT=yes.                                     |
+-----------------+----------------------------------------------------------------+
|  size           |  Print application size information.                           |
+-----------------+----------------------------------------------------------------+
|  help           |  Show the help.                                                |
+-----------------+----------------------------------------------------------------+

Variables
^^^^^^^^^

There are plenty of make variables used to control the build
process. Below is a list of the most frequently used variables. The
advanced user may read the make files in :github-tree:`make`.

+-----------------+--------------------------------------------------------------------------------------------------+
|  Name           |  Description                                                                                     |
+=================+==================================================================================================+
|  SIMBA_ROOT     |  Path to the `Simba` root folder.                                                                |
+-----------------+--------------------------------------------------------------------------------------------------+
|  BOARD          |  The ``BOARD`` variable selects which board to build for. It can be assigned |br|                |
|                 |  to one of the boards listed :doc:`here<../boards>`. For example, the command |br|               |
|                 |  to build for :doc:`Arduino Due<../boards/arduino_due>` is ``make BOARD=arduino_due``.           |
+-----------------+--------------------------------------------------------------------------------------------------+
|  BAUDRATE       |  Serial port baudrate used by console and run targets.                                           |
+-----------------+--------------------------------------------------------------------------------------------------+
|  SERIAL_PORT    |  Serial port used by console and run targets.                                                    |
+-----------------+--------------------------------------------------------------------------------------------------+
|  VERSION        |  The application version string. Usually on the form |br|                                        |
|                 |  <major>.<minor>.<revision>.                                                                     |
+-----------------+--------------------------------------------------------------------------------------------------+
|  SETTINGS_INI   |  Path to the settings file.                                                                      |
+-----------------+--------------------------------------------------------------------------------------------------+
|  INC            |  Include paths.                                                                                  |
+-----------------+--------------------------------------------------------------------------------------------------+
|  SRC            |  Source files (.c, .asm, .rs).                                                                   |
+-----------------+--------------------------------------------------------------------------------------------------+
|  CFLAGS_EXTRA   |  Extra flags passed to the compiler.                                                             |
+-----------------+--------------------------------------------------------------------------------------------------+
|  LDFLAGS_EXTRA  |  Extra flags passed to the linker.                                                               |
+-----------------+--------------------------------------------------------------------------------------------------+
|  NASSERT        |  Build the application without assertions.                                                       |
+-----------------+--------------------------------------------------------------------------------------------------+

PlatformIO
----------

This section describes Simba specific usage of the PlatformIO build
system. Consult the `PlatformIO documentation`_ for a general
description.

Application name
^^^^^^^^^^^^^^^^

To set the application name, create a file called ``config.py`` in the
same folder as your projects ``platformio.ini``. The contents of
``config.py`` can be seen below, and you can change
``MyApplicationName`` to the name of your application.

.. code-block:: python

   Import('env')

   # Set the Simba application name.
   env.Replace(NAME='MyApplicationName')

Then add ``extra_scripts = config.py`` to your ``platformio.ini``, as
in the example below.

.. code-block:: ini

   [env:my_project]
   platform = atmelavr
   framework = simba
   board = uno
   extra_scripts = config.py

.. |br| raw:: html

   <br />

.. _PlatformIO documentation: http://docs.platformio.org/en/latest/projectconf.html
