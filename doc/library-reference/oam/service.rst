:mod:`service` --- Services
===========================

.. module:: service
   :synopsis: services.

A service is as a background task. A service is either running or
stopped.

Debug file system commands
--------------------------

Three debug file system commands is available, all located in the
directory ``oam/service/``.

+-------------------------------+-----------------------------------------------------------------+
|  Command                      | Description                                                     |
+===============================+=================================================================+
|  ``list``                     | List all registered services.                                   |
+-------------------------------+-----------------------------------------------------------------+
|  ``start <service>``          | Start given service.                                            |
+-------------------------------+-----------------------------------------------------------------+
|  ``stop <service>``           | Stop given service.                                             |
+-------------------------------+-----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

   $ oam/service/list
   NAME                   STATUS
   http_server            running
   ftp_server             stopped
   network_manager        running
   $ oam/service/start ftp_server
   $ oam/service/stop http_server
   $ oam/service/list
   NAME                   STATE
   http_server            stopped
   ftp_server             running
   network_manager        running

----------------------------------------------

Source code: :github-blob:`src/oam/service.h`, :github-blob:`src/oam/service.c`

Test code: :github-blob:`tst/oam/service/main.c`

Test coverage: :codecov:`src/oam/service.c`

----------------------------------------------

.. doxygenfile:: oam/service.h
   :project: simba
