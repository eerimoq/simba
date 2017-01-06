#!/usr/bin/env python3

import socket
import ssl


def recvall(sock, length):
    buf = b''
    while len(buf) < length:
        byte = sock.recv()
        if not byte:
            break
        buf += byte
    return buf


context = ssl.SSLContext(ssl.PROTOCOL_TLSv1)
context.load_cert_chain(certfile="server.crt",
                        keyfile="server.key")

listener_sock = socket.socket()
listener_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
listener_sock.bind(('', 10023))
listener_sock.listen(5)

while True:
    client_sock, fromaddr = listener_sock.accept()
    ssl_client_sock = context.wrap_socket(client_sock, server_side=True)
    try:
        print(recvall(ssl_client_sock, 6))
        print('write: Goodbye!')
        ssl_client_sock.send(b'Goodbye!')
    finally:
        ssl_client_sock.shutdown(socket.SHUT_RDWR)
        ssl_client_sock.close()
