:mod:`soam` --- Simba OAM
=========================

.. module:: soam
   :synopsis: Simba OAM.

Simba Operation And Maintenence (SOAM) is a framed debug protocol with
enumerated format strings and file system commands. This both saves
memory and makes the communication more reliable.

Two macros are defined; ``OSTR()`` and ``CSTR()``, both required by
the SOAM build system. It is considered good practice to always use
these macros, even if SOAM is not used.

- The ``OSTR()`` macro.

  An output format string.

  .. code-block:: c

     /* Log object. */
     log_object_print(NULL, LOG_INFO, OSTR("Hello %s!\r\n"), "Erik");

     /* File system command output. */
     static int cmd_foo_cb(...)
     {
         std_fprintf(chout_p, OSTR("Foo %d!\r\n"), 1);

         return (0);
     }

     /* Regular printf. */
     std_printf(OSTR("Hello 0x%x!\r\n"), 0xbabe);

- The ``CSTR()`` macro.

  A file system command string.

  .. code-block:: c

     fs_command_init(&cmd_foo, CSTR("/foo"), cmd_foo_cb, NULL);

Usage
-----

Enable SOAM by adding ``SOAM=yes`` to the application makefile.

Connect to the board with ``soam.py`` instead of a serial terminal
program.

The string database is called
``build/<board>/<application-name>.soamdb``.

----------------------------------------------

Source code: :github-blob:`src/oam/soam.h`, :github-blob:`src/oam/soam.c`

Test code: :github-blob:`tst/oam/soam/main.c`

Test coverage: :codecov:`src/oam/soam.c`

Example code: :github-blob:`examples/soam/main.c`

----------------------------------------------

.. doxygenfile:: oam/soam.h
   :project: simba
