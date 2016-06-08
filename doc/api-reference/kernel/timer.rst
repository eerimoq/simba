:mod:`timer` --- Timers
=======================

.. module:: timer
   :synopsis: Timers.

Timers are started with a given timeout, and when the timer expires
the timer callback function is called from interrupt context.

The timeout resolution is the system tick period. Timeouts are always
rounded up to the closest system tick. Tht is, a timer will never
expire early, but may expire slightly late.

An application requiring timers with higher precision than the system
tick must use the hardware timers.

----------------------------------------------

Source code: :github-blob:`src/kernel/kernel/timer.h`

Test code: :github-blob:`tst/kernel/timer/main.c`

----------------------------------------------

.. doxygenfile:: kernel/timer.h
   :project: simba
