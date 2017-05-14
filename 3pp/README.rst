Third party products
====================

This folder contains third party products and wrappers.

ESP8266 RTOS SDK
----------------

Version: 1.3.0

Github: https://github.com/espressif/ESP8266_RTOS_SDK/tree/50459308c11946c5ab28e2b7d30809b5e0956ee4

lib/libhal.a copied from Arduino repository.

Symbols redefinition
^^^^^^^^^^^^^^^^^^^^

This redefines various symbols so no name clashes between the SDK and
Simba occurs when building.

.. code-block:: text

   # Find all symbols in lib/ and craete ``redefine-symbols.txt``
   ./redefine-symbols-esp8266-rtos-sdk.py --find-symbols-in-libraries

   # Use ``redefine-symbols.txt`` to redefine symbols in header and
   # library files.
   ./redefine-symbols-esp8266-rtos-sdk.py
