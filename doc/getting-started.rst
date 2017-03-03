Getting Started
===============

Installation
------------

There are three build systems available; `PlatformIO`, `Arduino IDE`
and `Simba build system`. The `Simba build system` has more features
than to the other two. It supports executing test suites, generating
code coverage, profiling and more. Still, if you are familiar with
`Arduino IDE` or `PlatformIO`, use that instead since it will be less
troublesome.

|platformio| PlatformIO
^^^^^^^^^^^^^^^^^^^^^^^

Install `Simba` in `PlatformIO`_.

1. Install the `PlatformIO IDE`_.

2. Start the `PlatformIO IDE` and open `PlatformIO` -> `Project
   Examples` and select `simba/blink`.

3. Click on `Upload` (the arrow image) in the top left corner.

4. The built-in LED blinks!

5. Done!

|arduino| Arduino IDE
^^^^^^^^^^^^^^^^^^^^^

Install `Simba` in the `Arduino IDE 1.6.10`_ as a third party board using the
Boards Manager.

1. Open `File` -> `Preferences`.

2. Add these URL:s to `Additional Boards Manager URLs` (click on the
   icon to the right of the text field) and press `OK`.

   .. code-block:: text

      https://raw.githubusercontent.com/eerimoq/simba-releases/master/arduino/avr/package_simba_avr_index.json
      https://raw.githubusercontent.com/eerimoq/simba-releases/master/arduino/sam/package_simba_sam_index.json
      https://raw.githubusercontent.com/eerimoq/simba-releases/master/arduino/esp/package_simba_esp_index.json
      https://raw.githubusercontent.com/eerimoq/simba-releases/master/arduino/esp32/package_simba_esp32_index.json

3. Open `Tools` -> `Board: ...` -> `Boards Manager...` and type
   `simba` in the search box.

4. Click on `Simba by Erik Moqivst version x.y.z` and click
   `Install` and press `Close`.

5. Open `Tools` -> `Board: ...` -> `Boards Manager...` and
   select one of the Simba boards in the list.

6. Open `File` -> `Examples` -> `Simba` -> `blink`.

7. Verify and upload the sketch to your device.

8. The built-in LED blinks!

9. Done!

|simba| Simba build system
^^^^^^^^^^^^^^^^^^^^^^^^^^

The `Simba` development environment can be installed on `Linux (Ubuntu
14)`.

1. Execute the one-liner below to install `Simba`.

   .. code-block:: text

      $ mkdir simba && \
        cd simba && \
        sudo apt install ckermit valgrind cppcheck cloc python python-pip doxygen git lcov && \
        sudo apt install avrdude gcc-avr binutils-avr gdb-avr avr-libc && \
        sudo apt install bossa-cli gcc-arm-none-eabi && \
        sudo apt install make unrar autoconf automake libtool gcc g++ gperf \
                             flex bison texinfo gawk ncurses-dev libexpat-dev \
                             python-serial sed libtool-bin pmccabe help2man \
                             python-pyelftools && \
        sudo pip install pyserial xpect readchar sphinx breathe sphinx_rtd_theme && \
        (git clone --recursive https://github.com/pfalcon/esp-open-sdk && \
         cd esp-open-sdk && \
         make) && \
        wget https://github.com/eerimoq/simba-releases/raw/master/arduino/esp32/tools/xtensa-esp32-elf-linux$(getconf LONG_BIT)-1.22.0-59.tar.gz && \
        tar xf xtensa-esp32-elf-linux$(getconf LONG_BIT)-1.22.0-59.tar.gz && \
        rm xtensa-esp32-elf-linux$(getconf LONG_BIT)-1.22.0-59.tar.gz && \
        git clone --recursive https://github.com/eerimoq/simba

2. Setup the environment.

   .. code-block:: text

      $ cd simba
      $ source setup.sh

2. Build and upload the blink example to your device.

   .. code-block:: text

      $ cd examples/blink
      $ make -s BOARD=nano32 upload

3. The built-in LED blinks!

4. Done!

.. _Cygwin: https://cygwin.com/setup-x86.exe
.. _PlatformIO: http://platformio.org
.. _Arduino IDE 1.6.10: https://www.arduino.cc/en/Main/Software
.. _PlatformIO IDE: http://platformio.org/platformio-ide

.. |arduino| image:: images/Arduino_Logo.png
             :width: 40 px
.. _arduino: http://arduino.cc

.. |platformio| image:: images/platformio-logo.png
                :width: 40 px
.. _platformio: http://platformio.org

.. |simba| image:: images/logo.jpg
                :width: 80 px
.. _simba: http://github.com/eerimoq/simba
