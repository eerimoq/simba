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

- Connect DTR on the serial adapter to the RST pin on the Photon.

- Connect RTS on the serial adapter to the SETUP pad on the bottom
  side of the Photon. This requires soldering a cable to the SETUP
  pad.

Upload the software with ``make BOARD=photon upload``.
