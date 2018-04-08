Ping
====

About
-----

Ping a remote host periodically once every second.

See the :doc:`Ping Library Reference<../library-reference/inet/ping>`
for more details.

Source code
-----------

.. include:: ping/source-code.rst

The source code can also be found on Github in the
:github-tree:`examples/ping` folder.

Build and run
-------------

Build and run the application.

.. code-block:: text

   $ cd examples/ping
   $ make -s BOARD=<board> SSID=<wifi SSID> PASSWORD=<wifi password> run
   Successfully pinged '8.8.4.4' in 20 ms (#1).
   Successfully pinged '8.8.4.4' in 20 ms (#2).
   Successfully pinged '8.8.4.4' in 20 ms (#3).
