Installation
============

The `Simba` development environment can be installed on `Windows
(Cygwin)`_ and `Linux (Ubuntu 14)`_. Follow the installation procedure
below for your OS.

Windows (Cygwin)
----------------

Download `Cygwin`_ and select the following packages for installation:

.. code-block:: text

   - gcc-core          (Devel -> gcc-core)
   - make              (Devel -> make)
   - python            (Python -> python)
   - python-setuptools (Python -> python-setuptools)
   - git               (Devel -> git)
   - doxygen           (Devel -> doxygen)

Start `Cygwin` and execute the following commands to download the
`Simba` source code, install the prerequisities, setup the development
environment and run the hello world application. Note that ``source
setup.sh`` has to be executed everytime you start a new `Cygwin`
shell.

.. code-block:: text

   $ easy_install-2.7 pip && \
     pip install pyserial xpect sphinx breathe && \
     git clone https://github.com/eerimoq/avr-toolchain-windows && \
     git clone https://github.com/eerimoq/arm-toolchain-windows && \
     git clone https://github.com/eerimoq/simba && \
     cd simba && \
     source setup.sh && \
     cd examples/hello_world && \
     make -s run

Linux (Ubuntu 14)
--------------

Execute the following commands to download the `Simba` source code,
install the prerequisities, setup the development environment and run
the hello world application. Note that ``source setup.sh`` has to be
executed everytime you start a shell.

.. code-block:: text

   $ sudo apt-get install ckermit valgrind cppcheck cloc python python-pip doxygen && \
     sudo apt-get install avrdude gcc-avr binutils-avr gdb-avr avr-libc && \
     sudo apt-get install bossa-cli gcc-arm-none-eabi && \
     pip install pyserial xpect sphinx breathe && \
     git clone https://github.com/eerimoq/simba && \
     cd simba && \
     source setup.sh && \
     cd examples/hello_world && \
     make -s run

.. _Cygwin: https://cygwin.com/setup-x86.exe
