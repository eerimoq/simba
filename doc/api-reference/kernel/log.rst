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

----------------------------------------------

Source code: :github-blob:`src/kernel/kernel/log.h`

Test code: :github-blob:`tst/kernel/log/main.c`

----------------------------------------------

.. doxygenfile:: kernel/log.h
   :project: simba
