:mod:`log` --- Logging
======================

.. module:: log
   :synopsis: Logging.

The logging module consists of log objects and log handlers. A log
object filters log entries and a log handler writes log entries to an
output channel.

A log object called "log" and a log handler writing to standard output
are created during the log module initialization. The log handler can
be replaced by calling `log_set_default_handler_output_channel()`.

Normally one log object is created for each subsystem in an
application. This gives the user the power to control which parts of
the system to debug and/or monitor at runtime.

Sometimes it's useful to write log entries to multiple channels. This
is possible by creating and adding another log handler to the log
module.

The log entry format is:

.. code:: text

   <timestamp>:<log level>:<thread name>:<log object name>: <message>

A few example outputs using three log objects; foo, bar and the
default log object main. All logs are from the main thread as can be
seen in the third field in the entries.

.. code:: text

   23:info:main:foo: A foo info message.
   24:info:main:bar: A bar info message.
   37:debug:main:bar: A bar debug message.
   56:error:main:main: A main error message.

----------------------------------------------

Source code: :github-blob:`src/debug/log.h`

Test code: :github-blob:`tst/debug/log/main.c`

----------------------------------------------

.. doxygenfile:: debug/log.h
   :project: simba
