:mod:`time` --- System time
===========================

.. module:: time
   :synopsis: System time.

This module implements wall clock time, date and low overhead
microsecond timing functions.

The ``time_micros*()`` functions are intended for bit banging drivers,
requiring precise microsecond timing with very low overhead. The
microsecond counter wraps quite frequently, and it's recommended to
only measure very short time periods. The maximum time that can be
measured is port specific, and can be read at runtime with
``time_micros_maximum()``.

----------------------------------------------

Source code: :github-blob:`src/kernel/time.h`, :github-blob:`src/kernel/time.c`

Test code: :github-blob:`tst/kernel/time/main.c`

Test coverage: :codecov:`src/kernel/time.c`

----------------------------------------------

.. doxygenfile:: kernel/time.h
   :project: simba
