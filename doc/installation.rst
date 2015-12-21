Installation
============

Prerequisites
-------------

Required:

* GNU make 3.81
* python 2.7
* GNU toolchain with c compiler and linker for target architecture(s)
* gcc
* bash

Optional:

* valgrind
* cppcheck
* cloc

Linux (Ubuntu)
--------------

``sudo apt-get install ckermit valgrind cppcheck cloc python``

Avr
~~~

``sudo apt-get install avrdude gcc-avr binutils-avr gdb-avr avr-libc``

Arm
~~~

``sudo apt-get install bossa-cli gcc-arm-none-eabi``

Windows
-------

Download cygwin from `cygwin.com` and select the following packages:

* gcc-core
* make
* python
* git
* gitk

Directories
~~~~~~~~~~~

This is the directory structure expected by the cygwin setup script
``cygwin_setup.sh``:

.. code-block:: c

  -- {root}
     +-- simba/                    - Simba.
     +-- arm-toolchain-windows/    - ARM toolchain.
     +-- avr-toolchain-windows/    - Atmel AVR toolchain.

Avr
~~~

Download the Atmel AVR toolchain here:
https://github.com/eerimoq/avr-toolchain-windows/archive/master.zip

Unpack it in the same folder as ``simba/``. Then run ``source
cygwin_setup.sh`` in ``simba/`` to add the bin directory to the path.

Arm
~~~

Download the ARM toolchain here:
https://github.com/eerimoq/arm-toolchain-windows/archive/master.zip

Unpack it in the same folder as ``simba/``. Then run ``source
cygwin_setup.sh`` in ``simba/`` to add the bin directory to the path.
