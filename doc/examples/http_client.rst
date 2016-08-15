HTTP Client
===========

About
-----

Conenct to a remote host perform a HTTP GET action to fetch the root
page '/' from the remote host.

Define ``CONFIG_START_NETWORK_INTERFACE_WIFI_SSID`` and
``CONFIG_START_NETWORK_INTERFACE_WIFI_PASSWORD`` in ``config.h`` to
the SSID and password of your WiFi, otherwise the board will fail to
connect to the WiFi network. Alternatively, the defines can be given
as defines on the make command line as seen in the example below.

Source code
-----------

.. include:: http_client/source-code.rst

The source code can also be found on Github in the
:github-tree:`examples/http_client` folder.

Build and run
-------------

Build and run the application. It must be built for ESP12E or ESP01
since those are the only boards with a network connection (WiFi).

.. code-block:: text

   $ cd examples/http_client
   $ make -s BOARD=esp12e CDEFS_EXTRA="CONFIG_START_NETWORK_INTERFACE_WIFI_SSID=Qvist CONFIG_START_NETWORK_INTERFACE_WIFI_PASSWORD=FooBar" run
   ...
   Connecting to WiFi with SSID 'Qvist'.
   Connected to WiFi with SSID 'Qvist'. Got IP address '192.168.1.103'.
   Connecting to '216.58.211.142'.
   First 64 bytes of the response:
   HTTP/1.1 301 Moved Permanently
   Location: http://www.google.com/GET / HTTP/1.1
   Host: 216.58.211.142
   ...
   $
