Driver testing
==============

Driver testing may, or may not, require the real hardware. Both
alternatives have benefits and drawbacks.

A driver test suite that requires the real hardware cannot be run by
someone not having access to real hardware, obviously. On the other
hand, testing with real hardware ensures that the driver is compatible
with the real hardware. That is very important!

A driver test suite that does **not** require real hardware can be run
by anyone. This allows anyone to confidently make changes to a driver
without breaking legacy. It's also easier to test error paths in the
code since you can easily fake any hardware behaviour. Other benefits
are code coverage statistics and easier debugging with host tools.

It's up to the driver author to write the test suite(s) that fits the
drivers needs.

Folder structure:

.. code-block:: text

   -- drivers/
      +-- hardware/         # Test suites requiring real hardware.
      +-- software/         # Test suites **not** requiring real hardware.
