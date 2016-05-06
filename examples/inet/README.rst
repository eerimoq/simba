Overview
========

Send TCP and UCP traffic between a PC and an ESP8266.

The ESP8266 will automatically connect to a WiFi at startup, using
given SSID and PASSWORD defines.

Build and run
=============

1. Build and run the applications on the ESP8266.

.. code-block:: text

    >>> make -s BOARD=esp12e CFLAGS_EXTRA="-DSSID=my_ssid -DPASSWORD=my_password" release run

2. Run the PC script.

.. code-block:: text

    >>> ./client.py --server-ip-address 192.168.1.103
    UDP
    ---
    opening socket
    binding to '':30303
    sending 'hello udp' to ('192.168.1.103', 30303)
    received 'hello udp' from ('192.168.1.103', 30303)
    closing socket

    TCP
    ---
    opening socket
    connecting to 192.168.1.103:40404
    sending 'hello tcp'
    received 'hello tcp'
    closing socket

3. Connect to the shell on port 50505.

.. code-block:: text

   >>> telnet 192.168.1.103 50505
   $
