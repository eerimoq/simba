Flashing
--------

If you wish to program your Photon with a custom firmware via USB,
you'll need to use this mode. This mode triggers the on-board
bootloader that accepts firmware binary files via the dfu-utility.

Prerequisities
^^^^^^^^^^^^^^

Install the dfu utility.

.. code:: text

   git clone git://git.code.sf.net/p/dfu-util/dfu-util
   cd dfu-util
   sudo apt-get build-dep dfu-util
   ./autogen.sh
   ./configure
   make
   sudo make install
   cd ..

Flasing sequence
^^^^^^^^^^^^^^^^

To enter DFU Mode:

1. Hold down the RESET and SETUP buttons.

2. Release only the RESET button, while holding down the SETUP button.

3. Wait for the LED to start flashing yellow (it will flash magenta
   first).

4. Release the SETUP button.

5. Upload the software:
   
      ``sudo dfu-util -d 2b04:d006-D -a 0 -s 0x08020000:leave -D
      firmware.bin``

Wifi driver
^^^^^^^^^^^

Use the WICED library from Broadcom to communicate with the WiFi
module. Requires an account on the Boardcom homepage?
