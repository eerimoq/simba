Porting
=======

Adding a new board
------------------

Often the board you want to use in your project is not yet supported
by `Simba`. If you are lucky, `Simba` is already ported to the MCU on
your board. Just create a folder with you board name in
:github-tree:`src/boards/` and populate it with the ``board.h``,
``board.c`` and ``board.mk``. Also, create board documentation in
:github-tree:`doc/boards/`, and add a pinout image in
:github-tree:`doc/images/boards/`.

The same files as a file tree:

.. code-block:: text

   simba/
   +-- doc/
   |   +-- boards/
   |   |   +-- <board-name>.rst
   |   +-- images/
   |       +-- boards/
   |           +-- <board-name>-pinout.jpg
   +-- src/
       +-- boards/
           +-- <board-name>.h
           +-- <board-name>.c
           +-- <board-name>.mk

If `Simba` is not ported to your MCU, the kernel and drivers has to be
ported.

Kernel
------

Porting the kernel is a matter of configuring the system tick timer
and inplement a few locking primitives. If you are familiar with your
CPU, the port can be implemented quickly.

A kernel port is roughly 400 lines of code.

Kernel ports are implemented in :github-tree:`src/kernel/ports`.

Drivers
-------

The required work to port the drivers depends of which drivers you are
intrested in. The more drivers you have to port, the longer time it
takes, obviously.

A drivers port is roughly 200 lines of code per driver.

Drivers ports are implemented in :github-tree:`src/drivers/ports`.
