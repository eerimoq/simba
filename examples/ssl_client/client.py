#!/usr/bin/env python3

import sys
import socket
import ssl
import pprint

server_sock = socket.socket()
server_sock.connect((sys.argv[1], 10023))

# Require a certificate from the server. We used a self-signed
# certificate so here ca_certs must be the server certificate itself.
context = ssl.SSLContext(ssl.PROTOCOL_TLSv1)
context.verify_mode = ssl.CERT_REQUIRED
context.load_verify_locations("server.crt")

ssl_server_sock = context.wrap_socket(server_sock)

print(repr(ssl_server_sock.getpeername()))
print(ssl_server_sock.cipher())
print(pprint.pformat(ssl_server_sock.getpeercert()))

print('write: Hello!')
ssl_server_sock.send(b"Hello!")
print('read:', ssl_server_sock.recv(8))
