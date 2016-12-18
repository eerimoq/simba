#!/usr/bin/env python

from __future__ import print_function

import socket
import ssl

listener_sock = socket.socket()
listener_sock.bind(('', 10023))
listener_sock.listen(5)

while True:
    client_sock, fromaddr = listener_sock.accept()
    ssl_client_sock = ssl.wrap_socket(client_sock,
                                      server_side=True,
                                      certfile="server.crt",
                                      keyfile="server.key")
    try:
        print(ssl_client_sock.read())
    finally:
        ssl_client_sock.shutdown(socket.SHUT_RDWR)
        ssl_client_sock.close()
