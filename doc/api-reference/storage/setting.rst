:mod:`setting` --- Persistent application settings
==================================================

.. module:: setting
   :synopsis: Persistent application settings.

Settings are stored in a non-volatile memory (NVM). In other words,
settings are perserved even if the board is power cycled.

Application settings are defined in an ini-file. A setting has a type,
size, address and default value, all defined in the ini-file. The
build system takes this file as input and generates the header and
source files needed by this module.

Supported types are:

- ``int8_t``

- ``int16_t``

- ``int32_t``

- ``string``

The size is the number of bytes of given type. For the standard
integer types the size must be the value returned by `sizeof()`. For
strings it is the length of the string, including null termination.

The address for each setting is defined by the user, starting at
address 0 and increasing from there.

The build system variable ``SETTINGS_INI`` contains the path to the
ini-file. Set this variable to the path of the application ini-file.

Example
-------

In this example the ini-file has one setting defined, foo.

.. code-block:: ini

   [values]
   foo = -4

   [types]
   foo = int8_t

   [addresses]
   foo = 0x00

   [sizes]
   foo = 1

The settings can be read and written from the application code. The
error handling has been left out to make the example easier to read.

.. code-block:: c

   int bar()
   {
       int8_t foo;

       /* Read the setting. */
       setting_read(&foo,
                    SETTING_FOO_ADDR,
                    SETTING_FOO_SIZE);

       foo -= 1;

       /* Write the setting. */
       setting_write(SETTING_FOO_ADDR,
                     &foo,
                     SETTING_FOO_SIZE);

       return (0);
   }

File system commands
--------------------

Four fs commands are available:

- ``list``: Print the current settings.

- ``reset``: Overwrite the current settings with the default values
  (ini-file values).

- ``read``: Read a setting.

- ``write``: Write a setting.

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

----------------------------------------------

Source code: :github-blob:`src/storage/setting.h`

Test code: :github-blob:`tst/storage/setting/main.c`

Test coverage: :codecov:`src/storage/setting.c`

----------------------------------------------

.. doxygenfile:: storage/setting.h
   :project: simba
