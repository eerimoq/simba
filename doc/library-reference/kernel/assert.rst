:mod:`assert` --- Assertions
============================

.. module:: assert
   :synopsis: Assertions.

An assertion tests a condition, and either returns an error or
stops the application.

There are three kind of assertions in Simba; ``ASSERT()``,
``FATAL_ASSERT()`` and ``PANIC_ASSERT()``.

- ``ASSERT()`` is configurable to either return an error code, or call
  the fatal callback.

- ``FATAL_ASSERT()`` always calls the fatal callback.

- ``PANIC_ASSERT()`` calls the panic function, which allows this
  assertion to be used in interrupt context.

Example usage
-------------

All assertion macros share the same prototype, in this example testing
one condition each.

.. code-block:: c

   ASSERT(x > 1);

   FATAL_ASSERT(y != 2);

   PANIC_ASSERT(z < 3);

----------------------------------------------

Source code: :github-blob:`src/kernel/assert.h`

----------------------------------------------

.. doxygenfile:: kernel/assert.h
   :project: simba
