Create key and certificate
==========================

.. code-block:: text

   $ openssl genrsa -des3 -out server.orig.key 2048
   $ openssl rsa -in server.orig.key -out server.key
   $ openssl req -new -key server.key -out server.csr
   $ openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt

Test with Python server and client
==================================

Terminal 1:

.. code-block:: text

   $ ./server.py
   Hello!

Terminal 2:

.. code-block:: text

   $ ./client.py
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
