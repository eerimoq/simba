:mod:`timer` --- Timers
=======================

.. module:: timer
   :synopsis: Timers.

Timers are started with a timeout, and when the time is up the timer
expires and the timer callback function is called from interrupt
context.

The timeout resolution is the system tick period. Timeouts are always
rounded up to the closest system tick. That is, a timer can never
expire early, but may expire slightly late.

An application requiring timers with higher precision than the system
tick must use the hardware timers.

----------------------------------------------

Source code: :github-blob:`src/kernel/timer.h`, :github-blob:`src/kernel/timer.c`

Test code: :github-blob:`tst/kernel/timer/main.c`

Test coverage: :codecov:`src/kernel/timer.c`

----------------------------------------------

.. doxygenfile:: kernel/timer.h
   :project: simba
