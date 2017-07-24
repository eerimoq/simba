:mod:`pin` --- Digital pins
===========================

.. module:: pin
   :synopsis: Digital pins.

Debug file system commands
--------------------------

Three debug file system commands are available, all located in the
directory ``drivers/pin/``. These commands directly access the pin
device registers, without using the pin driver object.

+-------------------------------+-----------------------------------------------------------------+
|  Command                      | Description                                                     |
+===============================+=================================================================+
|  ``set_mode <pin> <mode>``    | Set the mode of the pin ``<pin>`` to ``<mode>``, where |br|     |
|                               | ``<mode>`` is one of ``output``, ``output_open_drain``, |br|    |
|                               | ``output_open_drain_pull_up``, ``input``, |br|                  |
|                               | ``input_pull_up`` and ``input_pull_down``.                      |
+-------------------------------+-----------------------------------------------------------------+
|  ``read <pin>``               | Read current input value of the pin ``<pin>``. |br|             |
|                               | ``high`` or ``low`` is printed.                                 |
+-------------------------------+-----------------------------------------------------------------+
|  ``write <pin> <value>``      | Write the value ``<value>`` to given output pin ``<pin>``, |br| |
|                               | where  |br| ``<value>`` is one of ``high`` and ``low``.         |
+-------------------------------+-----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

   $ drivers/basic/pin/set_mode d2 output
   OK
   $ drivers/basic/pin/write d2 high
   OK
   $ drivers/basic/pin/write d2 low
   OK
   $ drivers/basic/pin/set_mode d3 input
   OK
   $ drivers/basic/pin/read d3
   low
   OK

----------------------------------------------

Source code: :github-blob:`src/drivers/basic/pin.h`, :github-blob:`src/drivers/basic/pin.c`

Test code: :github-blob:`tst/drivers/hardware/basic/pin/main.c`

----------------------------------------------

.. doxygenfile:: drivers/basic/pin.h
   :project: simba

.. |br| raw:: html

   <br />
