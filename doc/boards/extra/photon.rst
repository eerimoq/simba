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

Flashing
--------

To enter DFU Mode:

1. Hold down the RESET and SETUP buttons.

2. Release only the RESET button, while holding down the SETUP button.

3. Wait for the LED to start flashing yellow (it will flash magenta
   first).

4. Release the SETUP button.

Upload the software, ``make BOARD=photon upload``.

Wifi driver
-----------

Use the WICED library from Broadcom to communicate with the WiFi
module. Requires an account on the Boardcom homepage?
