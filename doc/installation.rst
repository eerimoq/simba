Installation
============

There are three build systems available; `PlatformIO`, `Arduino IDE`
and `Simba build system`. The `Simba build system` has more features
than to the other two. It supports executing test suites, generating
code coverage, profiling and more. Still, if you are familiar with
`Arduino IDE` or `PlatformIO`, use that instead since it will be less
troublesome.

|platformio| PlatformIO
-----------------------

Install `Simba` in `PlatformIO`_.

|arduino| Arduino IDE
---------------------

Install `Simba` in the `Arduino IDE 1.6.10`_ as a third party board using the
Boards Manager.

1. Open `"File"` -> `"Preferences"`.

2. Add these URL:s to `"Additional Boards Manager URLs"` (click on the
   icon to the right of the text field) and press `"OK"`.

   .. code-block:: text

      https://sourceforge.net/projects/simba-arduino/files/avr/package_simba_avr_index.json
      https://sourceforge.net/projects/simba-arduino/files/sam/package_simba_sam_index.json
      https://sourceforge.net/projects/simba-arduino/files/esp/package_simba_esp_index.json

3. Open `"Tools"` -> `"Board: ..."` -> `"Boards Manager..."` and type
   `"simba"` in the search box.

4. Click on `"Simba by Erik Moqivst version 4.1.0"` and click
   `"Install"` and press `"Close"`.

5. Open `"Tools"` -> `"Board: ..."` -> `"Boards Manager..."` and
   select one of the Simba boards in the list.

6. Open `File` -> `Examples` -> `Simba` -> `hello_world`.

7. Select the Arduino serial port at `"Tools"` -> `"Port: ..."`.

8. Open `"Tools"` -> `"Serial Monitor"` and change the baudrate to
   38400 in the bottom right corner.

9. Verify and upload the sketch to your device.

10. Done!

|simba| Simba build system
--------------------------

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
.. _Arduino IDE 1.6.10: https://www.arduino.cc/en/Main/Software

.. |arduino| image:: images/Arduino_Logo.png
             :width: 40 px
.. _arduino: http://arduino.cc

.. |platformio| image:: images/platformio-logo.png
                :width: 40 px
.. _platformio: http://platformio.org

.. |simba| image:: images/logo.jpg
                :width: 80 px
.. _simba: http://github.com/eerimoq/simba
