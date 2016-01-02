:mod:`setting` --- Persistent application settings
==================================================

.. module:: setting
   :synopsis: Persistent application settings.

Source code: :source:`src/kernel/kernel/setting.h`

Test code: :source:`tst/kernel/setting/main.c`

Settings are stored in a non-volatile memory (NVM). In other words,
settings are perserved even if the board is power cycled.

Application settings are declared in a ini-file. The type, size,
address and default value of each setting are be written to this
ini-file by the user, then the build system takes this file as input
and generates the header and source files required by this module.

Supported types are ``int8_t``, ``int16_t``, ``int32_t``, ``int64_t``,
``uint8_t``, ``uint16_t``, ``uint32_t``, ``uint64_t`` and ``string``.

The size is the number of bytes of given type. For standard integer
types the size must be the value returned by `sizeof()`. For strings
it is the length of the string, including null termination.

The address for each setting is selected by the user, starting at
address 0 and increasing from there.

The build system variable ``SETTINGS_INI`` contains the path to the
ini-file. The user may set this variable to the path of the desired
ini-file.

Here is an example ini-file that has one setting defined, foo.

.. code-block:: ini

   [values]
   foo = -4

   [types]
   foo = int8_t

   [addresses]
   foo = 0x00

   [sizes]
   foo = 1

And this is how to read and write the setting from the
application. The error handling has been left out to make the example
easier to read.

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

----------------------------------------------

.. doxygenfile:: kernel/setting.h
   :project: simba
