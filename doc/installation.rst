Installation
============

Windows
-------

Download cygwin from `cygwin.com` and select the following packages:

* gcc-core
* make
* python
* git
* gitk

Start Cygwin execute the following commands to download all software
and setup the environment. Note that ``source cygwin_setup.sh`` has to
be executed everytime you start a new Cygwin shell.

.. code-block:: c

   $ git clone https://github.com/eerimoq/simba
   $ git clone https://github.com/eerimoq/avr-toolchain-windows
   $ git clone https://github.com/eerimoq/arm-toolchain-windows
   $ cd simba
   $ source cygwin_setup.sh

Linux (Ubuntu)
--------------

.. code-block:: c

   $ git clone https://github.com/eerimoq/simba
   $ sudo apt-get install ckermit valgrind cppcheck cloc python
   $ sudo apt-get install avrdude gcc-avr binutils-avr gdb-avr avr-libc
   $ sudo apt-get install bossa-cli gcc-arm-none-eabi
