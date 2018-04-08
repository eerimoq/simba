HTTP Client
===========

About
-----

Conenct to a remote host perform a HTTP GET action to fetch the root
page '/' from the remote host.

See the :doc:`Socket Library Reference
<../library-reference/inet/socket>` for more details.

Source code
-----------

.. include:: http_client/source-code.rst

The source code can also be found on Github in the
:github-tree:`examples/http_client` folder.

Build and run
-------------

Build and run the application.

.. code-block:: text

   $ cd examples/http_client
   $ make -s BOARD=<board> SSID=<wifi SSID> PASSWORD=<wifi password> run
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
