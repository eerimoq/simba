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
``sync/service/``.

+-------------------------------+-----------------------------------------------------------------+
|  Command                      | Description                                                     |
+===============================+=================================================================+
|  ``list``                     | List all registered services.                                   |
+-------------------------------+-----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

   $ sync/service/list
   NAME
   http_server
   ftp_server

----------------------------------------------

Source code: :github-blob:`src/sync/service.h`, :github-blob:`src/sync/service.c`

Test code: :github-blob:`tst/sync/service/main.c`

Test coverage: :codecov:`src/sync/service.c`

Example code: :github-blob:`examples/service/main.c`

----------------------------------------------

.. doxygenfile:: sync/service.h
   :project: simba
