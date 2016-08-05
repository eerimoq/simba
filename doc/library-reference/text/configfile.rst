:mod:`configfile` --- Configuration file (INI-file)
===================================================

.. module:: configfile
   :synopsis: Configuration file.

The INI file format is an informal standard for configuration files
for some platforms or software. INI files are simple text files with a
basic structure composed of sections, properties, and values.

More information on Wikipedia_.

File format description
-----------------------

- Line terminators: ``\n``, ``\r\n`` or ``\n\r``.

- Opening bracket (``[``) at the beginning of a line indicates a
  section. The section name is all characters until a closing bracket
  (``]``).

- A property line starts with its name, then a colon (``:``) or equal
  sign (``=``), and then the value.

- Semicolon (``;``) or number sign (``#``) at the beginning of a line
  indicate a comment.

Example file
------------

.. code-block:: text

   ; last modified 1 April 2001 by John Doe
   [owner]
   name = John Doe
   organization = Acme Widgets Inc.

   [database]
   ; use IP address in case network name resolution is not working
   server = 192.0.2.62
   port = 143
   file = "payroll.dat"

----------------------------------------------

Source code: :github-blob:`src/text/configfile.h`, :github-blob:`src/text/configfile.c`

Test code: :github-blob:`tst/text/configfile/main.c`

Test coverage: :codecov:`src/text/configfile.c`

----------------------------------------------

.. doxygenfile:: text/configfile.h
   :project: simba

.. _Wikipedia: https://en.wikipedia.org/wiki/INI_file
