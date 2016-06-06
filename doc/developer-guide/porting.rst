Porting
=======

Often the board you want to use in your project is not yet supported
by `Simba`. If you are lucky the MCU on your board is already ported
it's very easy to add your board to :github-tree:`src/boards/`, but if
the kernel and drivers has to be ported more work is required.

Kernel
------

Porting the kernel is a matter of configuring the system tick timer
and inplement a few locking primitives. If you are familiar with your
CPU the port can be implemented quickly.

Kernel ports are implemented in :github-tree:`src/kernel/ports`.

Drivers
-------

The required work to port the drivers depends of which drivers you are
intrested in. The more drivers you have to port, the longer time it
takes, obviously.

Drivers ports are implemented in :github-tree:`src/drivers/ports`.
