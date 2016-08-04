:mod:`fs` --- Debug file system
===============================

.. module:: fs
   :synopsis: Debug file system.


File system commands
--------------------

Four file system commands are available, all located in the directory
``oam/fs/``.

+-------------------------------+-----------------------------------------------------------------+
|  Command                      | Description                                                     |
+===============================+=================================================================+
|  ``file_systems/list``        | Print a list of all registered file systems.                    |
+-------------------------------+-----------------------------------------------------------------+
|  ``counters/list``            | Print a list of all registered counters.                        |
+-------------------------------+-----------------------------------------------------------------+
|  ``counters/reset``           | Rest all counters to zero.                                      |
+-------------------------------+-----------------------------------------------------------------+
|  ``parameters/list``          | Print a list of all registered parameters.                      |
+-------------------------------+-----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

   $ oam/fs/file_systems/list
   MOUNT POINT                    MEDIUM   TYPE     AVAILABLE  SIZE  USAGE
   /tmp                           ram      fat16          54K   64K    14%
   /home/erik                     sd       fat16         1.9G    2G     5%
   /etc                           flash    spiffs        124K  128K     3%
   $ oam/fs/counters/list
   NAME                                                 VALUE
   /your/counter                                        0000000000000034
   /my/counter                                          0000000000000002
   $ oam/fs/counters/reset
   $ oam/fs/counters/list
   NAME                                                 VALUE
   /your/counter                                        0000000000000000
   /my/counter                                          0000000000000000
   $ oam/fs/parameters/list
   NAME                                                 VALUE
   /foo/bar                                             -2

----------------------------------------------

Source code: :github-blob:`src/oam/fs.h`

Test code: :github-blob:`tst/oam/fs/main.c`

Test coverage: :codecov:`src/oam/fs.c`

----------------------------------------------

.. doxygenfile:: oam/fs.h
   :project: simba
