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

Log levels
----------

There are five log levels defined; fatal, error, warning, info and
debug. The log levels are defined as ``LOG_<upper case level>`` in the
log module header file.

Log entry format
----------------

A log entry consists of a timestamp, log level, thread name, log
object name and the message. The timestamp is the log entry creation
time and the log level is one of fatal, error, warning, info and
debug. The thread name is the name of the thread that created the log
entry and the log object name is the name of the log object the entry
was printed on. The message is a user defined string.

.. code-block:: text

   <timestamp>:<log level>:<thread name>:<log object name>: <message>

Debug file system commands
--------------------------

Three debug file system commands are available, all located in the
directory ``debug/log/``.

+-----------------------------------+-----------------------------------------------------------------+
|  Command                          | Description                                                     |
+===================================+=================================================================+
|  ``list``                         | Print a list of all log objects.                                |
+-----------------------------------+-----------------------------------------------------------------+
|  ``print <string>``               | Print a log entry using the default log object and log |br|     |
|                                   | level ``LOG_INFO``. This command has no use except to test |br| |
|                                   | that the log module works.                                      |
+-----------------------------------+-----------------------------------------------------------------+
|  ``set_log_mask <object> <mask>`` | Set the log mask to ``<mask>`` for log object ``<object>``.     |
+-----------------------------------+-----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

    $ debug/log/list
         OBJECT NAME  MASK
             default  0x0f
    $ debug/log/print "Hello World!"
    $ debug/log/set_log_mask default 0x1f
    $ debug/log/list
         OBJECT NAME  MASK
             default  0x1f
    $ debug/log/print "Hello World!!!"
    56:info:main:default: Hello World!!!

Example
-------

Here is an example of how to create two log objects; `foo` and `bar`,
and then use them and the default log object `default`.

The source code:

.. code-block:: c

   /* Initialize the log objects foo and bar. */
   struct log_object_t foo;
   struct log_object_t bar;

   log_object_init(&foo, "foo", LOG_UPTO(INFO));
   log_object_init(&bar, "bar", LOG_UPTO(DEBUG));

   /* Print four log entries. */
   log_object_print(&foo, LOG_INFO, OSTR("A foo info message."));
   log_object_print(&bar, LOG_INFO, OSTR("A bar info message."));
   log_object_print(&bar, LOG_DEBUG, OSTR("A bar debug message."));
   log_object_print(NULL, LOG_ERROR, OSTR("A default error message."));

All logs are printed from the main thread as can be seen in the third
field in the entries in the output below.

.. code-block:: text

   23.0:info:main:foo: A foo info message.
   24.0:info:main:bar: A bar info message.
   37.0:debug:main:bar: A bar debug message.
   56.0:error:main:default: A default error message.

----------------------------------------------

Source code: :github-blob:`src/debug/log.h`, :github-blob:`src/debug/log.c`

Test code: :github-blob:`tst/debug/log/main.c`

Test coverage: :codecov:`src/debug/log.c`

----------------------------------------------

.. doxygenfile:: debug/log.h
   :project: simba

.. |br| raw:: html

   <br />
