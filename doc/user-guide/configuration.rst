Configuration
=============

Here is a list of all configuration variables available in
`Simba`. These are used to customize `Simba` for your application
needs.

By default, the configuration file :github-blob:`src/config.h` is used
when building an application. Create a file with the same name,
``config.h``, in your application folder to override the default
configuration.

The configuration variables can also be set on the command line as
``CFLAGS_EXTRA="-D<configuration variable>=<1 or 0>``.

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
