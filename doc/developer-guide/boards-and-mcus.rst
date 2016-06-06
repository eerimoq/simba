Boards and mcus
===============

A board is the top level configuration entity in the build
framework. It contains information about the MCU and the pin mapping.

In turn, the MCU contains information about available devices and
clock frequencys in the microcontroller.

See :github-tree:`src/boards/` and :github-tree:`src/mcus` for
available configurations.

Only one MCU per board is supported. If there are two MCU:s on one
physical board, two board configurations have to be created, one for
each MCU.

The porting guide :doc:`porting` shows how to port `Simba` to a new
board.
