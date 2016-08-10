:mod:`fs` --- Debug file system
===============================

.. module:: fs
   :synopsis: Debug file system.

The debug file system is not really a file system, but rather a file
system like tree of commands, counters, parameters, and "real" file
systems.

- A command is a file path mapped to a function callback. The callback
  is invoked when its path is passed to the ``fs_call()``
  function. Commands are registered into the debug file system by a
  call to ``fs_command_register()``.

- A counter is a file path mapped to a 64 bit value. The value can be
  incremented and read by the application. Counters are registered
  into the debug file system by a call to ``fs_counter_register()``.

- A parameter is file path mapped to a value stored in ram that can be
  easily read and modified by the user from a shell. Parameters are
  registered into the debug file system by a call to
  ``fs_parameter_register()``.

- A "real" file system is a file path, or mount point, mapped to a
  file system instance. The debug file system has a file access
  interface. The purpose of this interface is to have a common file
  access interface, independent of the underlying file systems
  interface. File systems are registered into the debug file system by
  a call to ``fs_filesystem_register()``.

Debug file system commands
--------------------------

The debug file system module itself registers seven commands, all
located in the directory ``filesystems/fs/``.

+---------------------------------------+---------------------------------------------------------+
|  Command                              | Description                                             |
+=======================================+=========================================================+
|  ``filesystems/list``                 | Print a list of all registered file systems.            |
+---------------------------------------+---------------------------------------------------------+
|  ``list [<folder>]``                  | Print a list of all files and folders in given folder.  |
+---------------------------------------+---------------------------------------------------------+
|  ``read <file>``                      | Read from given file.                                   |
+---------------------------------------+---------------------------------------------------------+
|  ``write <file> <data>``              | Create and write to a file. Overwrites existing files.  |
+---------------------------------------+---------------------------------------------------------+
|  ``append <file> <data>``             | Append data to an existing file.                        |
+---------------------------------------+---------------------------------------------------------+
|  ``counters/list``                    | Print a list of all registered counters.                |
+---------------------------------------+---------------------------------------------------------+
|  ``counters/reset``                   | Rest all counters to zero.                              |
+---------------------------------------+---------------------------------------------------------+
|  ``parameters/list``                  | Print a list of all registered parameters.              |
+---------------------------------------+---------------------------------------------------------+

Example output from the shell:

.. code-block:: text

   $ filesystems/fs/filesystems/list
   MOUNT-POINT                    MEDIUM   TYPE     AVAILABLE  SIZE  USAGE
   /tmp                           ram      fat16          54K   64K    14%
   /home/erik                     sd       fat16         1.9G    2G     5%
   /etc                           flash    spiffs        124K  128K     3%
   $ filesystems/fs/write tmp/foo.txt "Hello "
   $ filesystems/fs/append tmp/foo.txt world!
   $ filesystems/fs/read tmp/foo.txt
   Hello world!
   $ filesystems/fs/list tmp
   xxxx-xx-xx xx-xx       12 foo.txt
   $ filesystems/fs/counters/list
   NAME                                                 VALUE
   /your/counter                                        0000000000000034
   /my/counter                                          0000000000000002
   $ filesystems/fs/counters/reset
   $ filesystems/fs/counters/list
   NAME                                                 VALUE
   /your/counter                                        0000000000000000
   /my/counter                                          0000000000000000
   $ filesystems/fs/parameters/list
   NAME                                                 VALUE
   /foo/bar                                             -2

----------------------------------------------

Source code: :github-blob:`src/filesystems/fs.h`, :github-blob:`src/filesystems/fs.c`

Test code: :github-blob:`tst/filesystems/fs/main.c`

Test coverage: :codecov:`src/filesystems/fs.c`

----------------------------------------------

.. doxygenfile:: filesystems/fs.h
   :project: simba
