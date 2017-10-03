:mod:`shell` --- Debug shell
============================

.. module:: shell
   :synopsis: Debug shell.

The shell is a command line interface where the user can execute
various commands to control, debug and monitor its application.

.. code-block:: text

   > make -s console
   $ kernel/sys/info
   app:    shell-master built 2017-03-05 21:26 CET by erik.
   board:  Arduino Due
   mcu:    Atmel SAM3X8E Cortex-M3 @ 84MHz, 96k sram, 512k flash
   OK
   $ kernel/thrd/list
               NAME        STATE  PRIO   CPU   SCHEDULED  MAX-STACK-USAGE  LOGMASK
            monitor    suspended   -80    0%          22       176/   518     0x0f
               idle        ready   127   99%         594       276/   390     0x0f
               main      current     0    0%         305       540/ 88898     0x00
   OK
   $ kernel/thrd/set_log_mask foo 0
   ERROR(-3)
   $ <Ctrl-D>
   >

The shell module has a few configuration variables that can be used to
tailor the shell to the application requirements. Most noticeably is
the configuration variable ``CONFIG_SHELL_MINIMAL``. If set to ``0``
all the shell functionality is built; including tab completion, cursor
movement, line editing and command history. If set to ``1`` only the
minimal functionality is built; only including tab completion and line
editing at the end of the line.

See :doc:`../../user-guide/configuration` for a list of all
configuration variables.

----------------------------------------------

Source code: :github-blob:`src/oam/shell.h`, :github-blob:`src/oam/shell.c`

Test code: :github-blob:`tst/oam/shell/main.c`

Test coverage: :codecov:`src/oam/shell.c`

Example code: :github-blob:`examples/shell/main.c`

----------------------------------------------

.. doxygenfile:: oam/shell.h
   :project: simba
