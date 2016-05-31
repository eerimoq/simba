Configuration
=============

Here is a list of all compile time configuration variables available
in `Simba`. These are used to customize `Simba` for your application
needs. The default values are defined in
:github-blob:`src/config_default.h`.

Search order
------------

Highest priority first.

1. Command line as ``CFLAGS_EXTRA="-D<configuration
   variable>=<value>"``.

2. A file named ``config.h`` in the application root folder.

3. The default configuration, :github-blob:`src/config_default.h`.

Variables
---------

+-------------------------------+-------------------------------------------+
|  Name                         | Description                               |
+===============================+===========================================+
|  CONFIG_ASSERT                | Build with assert.                        |
+-------------------------------+-------------------------------------------+
|  CONFIG_DEBUG                 | Build with debug.                         |
+-------------------------------+-------------------------------------------+
|  CONFIG_PROFILE_STACK         | Stack profiling.                          |
+-------------------------------+-------------------------------------------+
|  CONFIG_PREEMPTIVE_SCHEDULER  | Use a preemptive scheduler.               |
+-------------------------------+-------------------------------------------+
|  CONFIG_SYSTEM_TICK_FREQUENCY | System tick frequency in Hertz.           |
+-------------------------------+-------------------------------------------+
|  CONFIG_MONITOR_THREAD        | Start the monitor thread.                 |
+-------------------------------+-------------------------------------------+
