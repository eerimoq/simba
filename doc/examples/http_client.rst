HTTP Client
===========

Source code: :github-blob:`examples/http_client/main.c`

------------------------------------------------

About
-----

Conenct to a remote host perform a HTTP GET action to fetch the root
page '/' from the remote host.

Define SSID and PASSWORD to the SSID and password of your WiFi,
otherwise the board will fail to connect to a network.

Source code
-----------

.. include:: http_client/source-code.rst

Build and run
-------------

Build and run the application. It must be built for ESP12E or ESP01
since those are the only boards with a network connection (WiFi).

.. code-block:: text

   $ cd examples/http_client
   $ make -s BOARD=<board> run
