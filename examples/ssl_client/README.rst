Test with Python client and server
==================================

Terminal 1:

.. code-block:: text

   $ ./server.py
   b'Hello!'
   write: Goodbye!

Terminal 2:

.. code-block:: text

   $ ./client.py localhost
   ('127.0.0.1', 10023)
   ('ECDHE-RSA-AES256-SHA', 'TLSv1/SSLv3', 256)
   {'issuer': ((('countryName', 'AU'),),
               (('stateOrProvinceName', 'Some-State'),),
               (('organizationName', 'Internet Widgits Pty Ltd'),)),
    'notAfter': 'Dec 18 08:38:52 2017 GMT',
    'notBefore': 'Dec 18 08:38:52 2016 GMT',
    'serialNumber': 'F19EAC34D9ADFC7C',
    'subject': ((('countryName', 'AU'),),
                (('stateOrProvinceName', 'Some-State'),),
                (('organizationName', 'Internet Widgits Pty Ltd'),)),
    'version': 1}
   write: Hello!
   read: b'Goodbye!'
   $

Test with Simba client (on Nano32) and Python server
====================================================

Terminal 1:

.. code-block:: text

   $ ./server.py
   b'Hello!'
   write: Goodbye!

Terminal 2:

.. code-block:: text

   $ make -s -j4 BOARD=nano32 CDEFS_EXTRA="CONFIG_START_NETWORK_INTERFACE_WIFI_SSID=Qvist2 CONFIG_START_NETWORK_INTERFACE_WIFI_PASSWORD=maxierik" run
   ...
   Connecting to server at 192.168.0.4:10023...
   Connected.
   write: Hello!
   read: Goodbye!
   ...

Create key and certificate
==========================

.. code-block:: text

   $ openssl genrsa -des3 -out server.orig.key 2048
   $ openssl rsa -in server.orig.key -out server.key
   $ openssl req -new -key server.key -out server.csr
   $ openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt

