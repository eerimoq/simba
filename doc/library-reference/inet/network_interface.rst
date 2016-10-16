:mod:`network_interface` --- Network interface
==============================================

.. module:: network_interface
   :synopsis: Network interface configuration.

The network interface module has a list of all network interfaces and
their states.

Network interface modules:

.. toctree::
   :glob:
   :titlesonly:

   network_interface/*

Debug file system commands
--------------------------

One debug file system command is available, located in the directory
``inet/network_interface/``.

+----------------------------------------+----------------------------------------------------------------+
|  Command                               | Description                                                    |
+========================================+================================================================+
|  ``list``                              | Print a list of all registered network interfaces.             |
+----------------------------------------+----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

   $ inet/network_interface/list
   NAME            STATE  ADDRESS            TX BYTES    RX BYTES
   esp-wlan-ap     up     192.168.4.1               -           -
   esp-wlan-sta    up     192.168.0.5               -           -

----------------------------------------------

Source code: :github-blob:`src/inet/network_interface.h`, :github-blob:`src/inet/network_interface.c`

Test coverage: :codecov:`src/inet/network_interface.c`

----------------------------------------------

.. doxygenfile:: inet/network_interface.h
   :project: simba
