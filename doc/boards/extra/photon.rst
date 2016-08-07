Detailed pinout
---------------

Right side pins
^^^^^^^^^^^^^^^

.. image:: ../images/boards/photon-pinout1.png
   :width: 100%
   :target: ../_images/photon-pinout1.png

Left side pins
^^^^^^^^^^^^^^

.. image:: ../images/boards/photon-pinout2.png
   :width: 100%
   :target: ../_images/photon-pinout2.png

User I/O
^^^^^^^^

.. image:: ../images/boards/photon-pinout3.png
   :width: 100%
   :target: ../_images/photon-pinout3.png

Prerequisities
--------------

Install the dfu-utility.

.. code:: text

   git clone git://git.code.sf.net/p/dfu-util/dfu-util
   cd dfu-util
   sudo apt-get build-dep dfu-util
   ./autogen.sh
   ./configure
   make
   sudo make install
   cd ..

   # Give users access to the device.
   sudo cp simba/environment/udev/49-photon.rules /etc/udec/rules.d
   
Flashing
--------

The Photon must enter DFU mode before software can be uploaded to
it. It's recommended to use the manual method to verify that software
can be successfully uploaded to the board, and then start using the
automatic method to reduce the manual work for each software upload.

Automatic (recommended)
^^^^^^^^^^^^^^^^^^^^^^^

- Connect DTR on the serial adapter to the RST pin on the Photon.

- Connect RTS on the serial adapter to the SETUP pad on the bottom
  side of the Photon. This requires soldering a cable to the SETUP
  pad.

Upload the software with ``make BOARD=photon upload``.

Manual
^^^^^^

To enter DFU Mode:

1. Hold down the RESET and SETUP buttons.

2. Release only the RESET button, while holding down the SETUP button.

3. Wait for the LED to start flashing yellow (it will flash magenta
   first).

4. Release the SETUP button.

NOTE: Do **not** connect DTR and/or RTS using manual upload. They must
only be connected using the automatic method.

Upload the software with ``make BOARD=photon upload``.
