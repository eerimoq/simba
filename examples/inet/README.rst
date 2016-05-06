OVERVIEW
========

Send TCP and UCP traffic between a PC and an ESP8266.

The ESP8266 will automatically connect to a WiFi at startup, using
given SSID and PASSWORD defines.

BUILD AND RUN
=============

Build and run the applications on the ESP8266.

.. code-block:: text

    >>> make -s BOARD=esp12e CFLAGS_EXTRA="-DSSID=my_ssid -DPASSWORD=my_password" release run

Run the PC script.

.. code-block:: text

    >>> ./client.py --server-ip-address 192.168.1.103
    UDP
    ---
    opening socket
    binding to 192.168.1.106:30303
    sending 'hello udp' to 192.168.1.103:30303
    received 'hello udp' from 192.168.1.106:30303
    closing socket

    TCP
    ---
    opening socket
    connecting to 192.168.1.103:40404
    sending 'hello tcp'
    received 'hello tcp'
    closing socket

Connect to the shell on port 50505.

.. code-block:: text

   >>> telnet 192.168.1.107 50505
   $ 
