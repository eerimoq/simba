:mod:`settings` --- Persistent application settings
==================================================

.. module:: settings
   :synopsis: Persistent application settings.

Settings are stored in a non-volatile memory (NVM). In other words,
settings are perserved after a board reset or power cycle.

Application settings are defined in an ini-file that is used to
generate the c source code. A setting has a type, a size, an address
and a default value, all defined in the ini-file.

Supported types are:

- ``int32_t`` A 32 bits signed integer.

- ``string`` An ASCII string.

- ``blob`` A chunk of data.

The size is the number of bytes of the value. For the standard integer
types the size must be the value returned by `sizeof()`. For strings
it is the length of the string, including null termination.

The address for each setting is defined by the user, starting at
address 0 and increasing from there.

The build system variable ``SETTINGS_INI`` contains the path to the
ini-file used by the build system. Set this variable to the path of
the applications ini-file to automatically generate four files in the
board's generate directory ``build/<board>/gen``; ``settings.h``,
``settings.c``, ``settings.little-endian.bin`` and
``settings.big-endian.bin``. One can also generate the four files with
``make settings-generate``.

Include ``gen/settings.h`` in source files that uses the settings.

Debug file system commands
--------------------------

Four debug file system commands are available, all located in the
directory ``oam/settings/``.

+-------------------------------+-----------------------------------------------------------------+
|  Command                      | Description                                                     |
+===============================+=================================================================+
|  ``list``                     | Print a list of the current settings.                           |
+-------------------------------+-----------------------------------------------------------------+
|  ``reset``                    | Overwrite the current settings values with their default |br|   |
|                               | values (the values defined in the ini-file values).             |
+-------------------------------+-----------------------------------------------------------------+
|  ``read <name>``              | Read the value of setting ``<name>``.                           |
+-------------------------------+-----------------------------------------------------------------+
|  ``write <name> <value>``     | Write ``<value>`` to setting ``<name>``.                        |
+-------------------------------+-----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

   $ oam/settings/list 
   NAME                  TYPE      SIZE  VALUE
   version               int32_t      4  1
   value_1               int32_t      4  24567
   value_2               blob_t       4  \xca\xfe\xba\xbe
   value_3               string_t    16  foobar
   $ oam/settings/read value_1
   24567
   $ oam/settings/write value_1 -5
   $ oam/settings/read value_1
   -5
   $ oam/settings/reset
   $ oam/settings/list 
   NAME                  TYPE      SIZE  VALUE
   version               int32_t      4  1
   value_1               int32_t      4  24567
   value_2               blob_t       4  \xca\xfe\xba\xbe
   value_3               string_t    16  foobar

Example
-------

In this example the ini-file has one setting defined, ``foo``. The
type is ``int32_t``, the address is ``0x00``, the size is ``4`` and the
default value is ``-4``.

.. code-block:: ini

   [values]
   foo = -4

   [types]
   foo = int32_t

   [addresses]
   foo = 0x00

   [sizes]
   foo = 4

The settings can be read and written with the functions
`settings_read()` and `settings_write()`. Give the generated defines
``SETTING_FOO_ADDR`` and ``SETTING_FOO_SIZE`` as arguments to those
functions.

.. code-block:: c

   int my_read_write_foo()
   {
       int32_t foo;

       /* Read the foo setting. */
       if (settings_read(&foo,
                         SETTING_FOO_ADDR,
                         SETTING_FOO_SIZE) != 0) {
           return (-1);
       }

       foo -= 1;

       /* Write the foo setting. */
       if (settings_write(SETTING_FOO_ADDR,
                          &foo,
                          SETTING_FOO_SIZE) != 0) {
           return (-1);
       }

       return (0);
   }

----------------------------------------------

Source code: :github-blob:`src/oam/settings.h`, :github-blob:`src/oam/settings.c`

Test code: :github-blob:`tst/oam/settings/main.c`

Test coverage: :codecov:`src/oam/settings.c`

----------------------------------------------

.. doxygenfile:: oam/settings.h
   :project: simba

.. |br| raw:: html

   <br />
