Test with Python server and client
==================================

Terminal 1:

.. code-block:: text

   $ ./server.py
   Hello!

Terminal 2:

.. code-block:: text

   $ ./client.py localhost
   ('127.0.0.1', 10023)
   ('ECDHE-RSA-AES256-GCM-SHA384', 'TLSv1/SSLv3', 256)
   {'issuer': ((('countryName', u'AU'),),
               (('stateOrProvinceName', u'Some-State'),),
               (('organizationName', u'Internet Widgits Pty Ltd'),)),
    'notAfter': 'Dec 18 08:38:52 2017 GMT',
    'notBefore': u'Dec 18 08:38:52 2016 GMT',
    'serialNumber': u'F19EAC34D9ADFC7C',
    'subject': ((('countryName', u'AU'),),
                (('stateOrProvinceName', u'Some-State'),),
                (('organizationName', u'Internet Widgits Pty Ltd'),)),
    'version': 1L}
   read: Hello!
   $

Test with Simba server (on Nano32) and Firefox
==============================================

Terminal:

.. code-block:: text

   $ make -s -j4 BOARD=nano32 CDEFS_EXTRA="CONFIG_START_NETWORK_INTERFACE_WIFI_SSID=Qvist2 CONFIG_START_NETWORK_INTERFACE_WIFI_PASSWORD=maxierik" run
   ...
   Waiting for a client to connect to 192.168.0.7:10023...
   Client accepted.
   read: 'GET / '
   ...

Firefox:

Enter the URL https://192.168.0.7:10023, accept the non-trusted
certificate, and 'Goodbye!' should appear on the screen.

Test with Simba server (on Nano32) and Python client
====================================================

Terminal 1:

.. code-block:: text

   $ make -s -j4 BOARD=nano32 CDEFS_EXTRA="CONFIG_START_NETWORK_INTERFACE_WIFI_SSID=Qvist2 CONFIG_START_NETWORK_INTERFACE_WIFI_PASSWORD=maxierik" run
   ...
   Waiting for a client to connect to 192.168.0.7:10023...
   Client accepted.
   read: 'Hello!'
   ...

Terminal 2:

.. code-block:: text

   $ ./client.py 192.168.0.8
   ('192.168.0.8', 10023)
   ('ECDHE-RSA-AES256-GCM-SHA384', 'TLSv1/SSLv3', 256)
   {'issuer': ((('countryName', u'AU'),),
               (('stateOrProvinceName', u'Some-State'),),
               (('organizationName', u'Internet Widgits Pty Ltd'),)),
    'notAfter': 'Dec 18 08:38:52 2017 GMT',
    'notBefore': u'Dec 18 08:38:52 2016 GMT',
    'serialNumber': u'F19EAC34D9ADFC7C',
    'subject': ((('countryName', u'AU'),),
                (('stateOrProvinceName', u'Some-State'),),
                (('organizationName', u'Internet Widgits Pty Ltd'),)),
    'version': 1L}
   read: Hello!
   $

Create key and certificate
==========================

.. code-block:: text

   $ openssl genrsa -des3 -out server.orig.key 2048
   $ openssl rsa -in server.orig.key -out server.key
   $ openssl req -new -key server.key -out server.csr
   $ openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt

