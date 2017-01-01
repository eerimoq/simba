#!/usr/bin/env python

from __future__ import print_function

import sys
import socket
import ssl
import pprint

server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Require a certificate from the server. We used a self-signed
# certificate so here ca_certs must be the server certificate itself.
ssl_server_sock = ssl.wrap_socket(server_sock,
                                  ca_certs="server.crt",
                                  cert_reqs=ssl.CERT_REQUIRED)

ssl_server_sock.connect((sys.argv[1], 10023))

print(repr(ssl_server_sock.getpeername()))
print(ssl_server_sock.cipher())
print(pprint.pformat(ssl_server_sock.getpeercert()))

ssl_server_sock.write("Hello!")
print('read:', ssl_server_sock.read(6))
