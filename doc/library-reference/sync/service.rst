:mod:`service` --- Services
===========================

.. module:: service
   :synopsis: services.

A service is as a piece of software that waits for events on one or
more channels, and when an event occurs the service callback is called
by the service server the service is registered to.

The total memory usage of an application is reduced since many
services share the same thread stack instead of having one stack each.

Debug file system commands
--------------------------

One debug file system commands is available, located in the directory
``oam/service/``.

+-------------------------------+-----------------------------------------------------------------+
|  Command                      | Description                                                     |
+===============================+=================================================================+
|  ``list``                     | List all registered services.                                   |
+-------------------------------+-----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

   $ oam/service/list
   NAME                  SERVER
   http_server           service_server
   ftp_server            service_server
   shell                 shell_server

----------------------------------------------

Source code: :github-blob:`src/oam/service.h`, :github-blob:`src/oam/service.c`

Test code: :github-blob:`tst/oam/service/main.c`

Test coverage: :codecov:`src/oam/service.c`

Example code: :github-blob:`examples/service/main.c`

----------------------------------------------

.. doxygenfile:: oam/service.h
   :project: simba
