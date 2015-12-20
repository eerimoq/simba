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

### Avr

``sudo apt-get install avrdude gcc-avr binutils-avr gdb-avr avr-libc``

### Arm

``sudo apt-get install bossa-cli gcc-arm-none-eabi``

Windows
-------

Download cygwin from `cygwin.com` and select the following packages:

* gcc-core
* make
* python
* git
* gitk

### Avr

Download the AVR toolchain:

### Arm

Download the ARM toolchain:
