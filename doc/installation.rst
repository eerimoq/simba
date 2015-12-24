Installation
============

Simba can be installed on `Windows`_ and `Linux (Ubuntu 14)`_. Follow
the installation procedure below for your OS.

Windows
-------

Download `cygwin`_ and select the following packages for installation:

* gcc-core
* make
* python
* python-setuptools
* git
* gitk

Start Cygwin and execute the following commands to download the Simba
source code, install the prerequisities and setup the development
environment. Note that ``source cygwin_setup.sh`` has to be executed
everytime you start a new Cygwin shell.

.. code-block:: text

   $ easy_install-2.7 pip
   $ pip install pyserial
   $ pip install xpect
   $ git clone https://github.com/eerimoq/simba
   $ git clone https://github.com/eerimoq/avr-toolchain-windows
   $ git clone https://github.com/eerimoq/arm-toolchain-windows
   $ cd simba
   $ source cygwin_setup.sh

Linux (Ubuntu 14)
--------------

Execute the following commands to download the Simba source code,
install the prerequisities and setup the development environment. Note
that ``source setup.sh`` has to be executed everytime you start a
shell.

.. code-block:: text

   $ git clone https://github.com/eerimoq/simba
   $ sudo apt-get install ckermit valgrind cppcheck cloc python
   $ sudo apt-get install avrdude gcc-avr binutils-avr gdb-avr avr-libc
   $ sudo apt-get install bossa-cli gcc-arm-none-eabi
   $ cd simba
   $ source setup.sh

.. _cygwin: http://cygwin.com
