Installation
============

Arduino IDE
-----------

Install `Simba` in the `Arduino IDE`_ using the Boards Manager.

1. Open `"File"` -> `"Preferences"`.
2. Append
   ``https://raw.githubusercontent.com/eerimoq/simba/master/make/arduino/package_simba_index.json``
   to `"Additional Boards Manager URLs"`.
3. Press `"OK"`.
4. Open `"Tools"` -> `"Board: ..."` -> `"Boards Manager..."`.
5. Type `"simba"` in the search box.
6. Click on `"Simba by Erik Moqivst version 0.6.0"` and click `"Install"`.
7. Press `"Close"`.
7. Open `"Tools"` -> `"Board: ..."` -> `"Boards Manager..."` and select one
   of the Simba boards in the list.
8. Copy the :github-blob:`Hello World <examples/hello_world/main.c>`
   example into sketch.
9. Verify and upload the sketch to your device.
10. Done!

PlatformIO
----------

Install `Simba` in `PlatformIO`_.

Simba build system
------------------

The `Simba` development environment can be installed on `Windows
(Cygwin)`_ and `Linux (Ubuntu 14)`_. Just follow the steps below and
you'll be up and running in no time. =)

Windows (Cygwin)
^^^^^^^^^^^^^^^^

Download `Cygwin`_ and select the following packages for installation:

**NOTE**: ESP8266 is not supported in `Cygwin` because there is no toolchain available.

.. code-block:: text

   - gcc-core          (Devel -> gcc-core)
   - make              (Devel -> make)
   - python            (Python -> python)
   - python-setuptools (Python -> python-setuptools)
   - git               (Devel -> git)
   - doxygen           (Devel -> doxygen)

Start `Cygwin` and execute the one-liner below to install `Simba`.

.. code-block:: text

   $ mkdir simba && \
     cd simba && \
     easy_install-2.7 pip && \
     pip install pyserial xpect readchar sphinx breathe && \
     git clone https://github.com/eerimoq/avr-toolchain-windows && \
     git clone https://github.com/eerimoq/arm-toolchain-windows && \
     git clone https://github.com/eerimoq/simba
     
Linux (Ubuntu 14)
^^^^^^^^^^^^^^^^^

Execute the one-liner below to install `Simba`.

.. code-block:: text

   $ mkdir simba && \
     cd simba && \
     sudo apt-get install ckermit valgrind cppcheck cloc python python-pip doxygen git && \
     sudo apt-get install avrdude gcc-avr binutils-avr gdb-avr avr-libc && \
     sudo apt-get install bossa-cli gcc-arm-none-eabi && \
     sudo apt-get install make unrar autoconf automake libtool gcc g++ gperf \
                          flex bison texinfo gawk ncurses-dev libexpat-dev \
                          python-serial sed && \
     pip install pyserial xpect readchar sphinx breathe && \
     (git clone --recursive https://github.com/eerimoq/esp-open-sdk.git && \
      cd esp-open-sdk && \
      make STANDALONE=n) && \
     (git clone https://github.com/eerimoq/rust-bindgen.git && \
      cd rust-bindgen && \
      cargo build) && \
     git clone https://github.com/eerimoq/rust.git && \
     git clone https://github.com/eerimoq/simba

Post-install
^^^^^^^^^^^^

Let's build and run the hello world application to verify that the
installation was successful.

.. code-block:: text

   $ cd simba && \
     source setup.sh && \
     cd examples/hello_world && \
     make -s run

.. _Cygwin: https://cygwin.com/setup-x86.exe
.. _PlatformIO: http://platformio.org
.. _Arduino IDE: https://www.arduino.cc/en/Main/Software
