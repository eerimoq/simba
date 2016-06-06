Porting
=======

Often the board you want to use in your project is not yet supported
by `Simba`. If you are lucky, `Simba` is already ported to the MCU on
your board. Just create a folder with you board name in
:github-tree:`src/boards/` and populate it with the ``board.h``,
``board.c`` and ``board.mk``. If `Simba` is not ported to your MCU,
the kernel and drivers has to be ported.

Kernel
------

Porting the kernel is a matter of configuring the system tick timer
and inplement a few locking primitives. If you are familiar with your
CPU, the port can be implemented quickly.

A kernel port is roughly 300 lines of code.

Kernel ports are implemented in :github-tree:`src/kernel/ports`.

Drivers
-------

The required work to port the drivers depends of which drivers you are
intrested in. The more drivers you have to port, the longer time it
takes, obviously.

A drivers port is roughly 100 lines of code per driver.

Drivers ports are implemented in :github-tree:`src/drivers/ports`.
