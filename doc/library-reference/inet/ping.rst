:mod:`ping` --- Ping
====================

.. module:: ping
   :synopsis: Ping.

Debug file system commands
--------------------------

One debug file system command is available, located in the directory
``inet/ping/``.

+----------------------------------------+----------------------------------------------------------------+
|  Command                               | Description                                                    |
+========================================+================================================================+
|  ``ping <remote host>``                | Ping a remote host by given ip address.                        |
+----------------------------------------+----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

   $ inet/ping/ping 192.168.1.100
   Successfully pinged '192.168.1.100' in 10 ms.
   $ 

----------------------------------------------

Source code: :github-blob:`src/inet/ping.h`, :github-blob:`src/inet/ping.c`

Test code: :github-blob:`tst/inet/ping/main.c`

Test coverage: :codecov:`src/inet/ping.c`

----------------------------------------------

.. doxygenfile:: inet/ping.h
   :project: simba
