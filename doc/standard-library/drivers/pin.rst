:mod:`pin` --- Digital pins
===========================

.. module:: pin
   :synopsis: Digital pins.

File system commands
--------------------

Three file system commands are available, all located in the directory
``drivers/pin/``. These commands directly access the pin device
registers, without using the pin driver object.

- ``set_mode <pin> <mode>`` Set the mode of the pin `<pin>` to
  `<mode>`, where `<mode>` is one of ``output`` and ``input``.

- ``read <pin>`` Read current input or output value of the pin
  `<pin>`. ``high`` or ``low`` is printed.

- ``write <pin> <value>`` Write the value `<value>` to pin `<pin>`,
  where `<value>` is one of ``high`` and ``low``.

Example output from the shell:

.. code-block:: text

    $ drivers/pin/set_mode d2 output
    $ drivers/pin/read d2
    low
    $ drivers/pin/write d2 high
    $ drivers/pin/read d2
    high
    $ drivers/pin/set_mode d3 input
    $ drivers/pin/read d3
    low

----------------------------------------------

Source code: :github-blob:`src/drivers/pin.h`

Test code: :github-blob:`tst/drivers/pin/main.c`

----------------------------------------------

.. doxygenfile:: drivers/pin.h
   :project: simba
