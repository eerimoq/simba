:mod:`setting` --- Persistent application settings
==================================================

.. module:: setting
   :synopsis: Persistent application settings.

Settings are stored in a non-volatile memory (NVM). In other words,
settings are perserved even if the board is power cycled.

Application settings are defined in an ini-file. A setting has a type,
a size, an address and a default value, all defined in the
ini-file. The build system takes this file as input and generates c
header and source files needed by this module.

Supported types are:

- ``int8_t`` An 8 bits signed integer.

- ``int16_t`` A 16 bits signed integer.

- ``int32_t`` A 32 bits signed integer.

- ``string`` An ASCII string.

The size is the number of bytes of the value. For the standard integer
types the size must be the value returned by `sizeof()`. For strings
it is the length of the string, including null termination.

The address for each setting is defined by the user, starting at
address 0 and increasing from there.

The build system variable ``SETTINGS_INI`` contains the path to the
ini-file. Set this variable to the path of the application ini-file.

File system commands
--------------------

Four file system commands are available, all located in the directory
``storage/setting/``.

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

   $ storage/setting/list 
   NAME                  TYPE     SIZE  VALUE
   version               int8_t      1  1
   value_1               int16_t     2  24567
   value_2               int32_t     4  -57
   value_3               string     16  foobar
   $ storage/setting/read value_1
   24567
   $ storage/setting/write value_1 -5
   $ storage/setting/read value_1
   -5
   $ storage/setting/reset
   $ storage/setting/list 
   NAME                  TYPE     SIZE  VALUE
   version               int8_t      1  1
   value_1               int16_t     2  24567
   value_2               int32_t     4  -57
   value_3               string     16  foobar

Example
-------

In this example the ini-file has one setting defined, ``foo``. The
type is ``int8_t``, the address is ``0x00``, the size is ``1`` and the
default value is ``-4``.

.. code-block:: ini

   [types]
   foo = int8_t

   [addresses]
   foo = 0x00

   [sizes]
   foo = 1

   [values]
   foo = -4

The settings can be read and written with the functions
`setting_read()` and `setting_write()`. Give the generated defines
``SETTING_FOO_ADDR`` and ``SETTING_FOO_SIZE`` as arguments to those
functions.

.. code-block:: c

   int my_read_write_foo()
   {
       int8_t foo;

       /* Read the foo setting. */
       if (setting_read(&foo,
                        SETTING_FOO_ADDR,
                        SETTING_FOO_SIZE) != 0) {
           return (-1);
       }

       foo -= 1;

       /* Write the foo setting. */
       if (setting_write(SETTING_FOO_ADDR,
                         &foo,
                         SETTING_FOO_SIZE) != 0) {
           return (-1);
       }

       return (0);
   }

----------------------------------------------

Source code: :github-blob:`src/storage/setting.h`

Test code: :github-blob:`tst/storage/setting/main.c`

Test coverage: :codecov:`src/storage/setting.c`

----------------------------------------------

.. doxygenfile:: storage/setting.h
   :project: simba

.. |br| raw:: html

   <br />
