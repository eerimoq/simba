#!/usr/bin/env python

from __future__ import print_function

import socket
import sys
import time


UDP_PORT = 30303
TCP_PORT = 40404
TCP_PORT_WRITE_CLOSE = 40405
TCP_PORT_SIZES = 40406

UDP_STRING = "hello udp"
TCP_STRING = "hello tcp"


def test_udp(server_ip_address,
             dev,
             timeout):
    """Perform the UDP test.

    """

    # Wait for the UDP test to start.
    dev.expect("recvfrom\r\n", timeout=timeout)

    print("run.py: opening socket")
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    print("run.py: binding to '':{}".format(UDP_PORT))
    sock.bind(('', UDP_PORT))

    print("run.py: sending '{}' to ('{}', {})".format(UDP_STRING,
                                                      server_ip_address,
                                                      UDP_PORT))
    sock.sendto(UDP_STRING, (server_ip_address, UDP_PORT))

    string, address = sock.recvfrom(1024)
    print("run.py: received '{}' from {}".format(string, address))

    assert string == UDP_STRING

    # Send to reader thread.
    print("run.py: sending to reader '{}' to ('{}', {})".format(UDP_STRING[::-1],
                                                                server_ip_address,
                                                                UDP_PORT))
    sock.sendto(UDP_STRING[::-1], (server_ip_address, UDP_PORT))

    string, address = sock.recvfrom(1024)
    print("run.py: received from reader '{}' from {}".format(string, address))

    assert string == UDP_STRING[::-1]

    time.sleep(0.5)

    # Polled receive.
    print("run.py: sending '{}' to ('{}', {})".format(UDP_STRING,
                                                      server_ip_address,
                                                      UDP_PORT))
    sock.sendto(UDP_STRING, (server_ip_address, UDP_PORT))

    string, address = sock.recvfrom(1024)
    print("run.py: received '{}' from {}".format(string, address))

    assert string == UDP_STRING

    print("run.py: closing socket")
    sock.close()


def test_tcp(server_ip_address,
             dev,
             timeout):
    """Perform the TCP test.

    """

    print("run.py: opening socket")
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Wait for the TCP test to start.
    dev.expect("listening on {}\r\n".format(TCP_PORT),
               timeout=timeout)

    print("run.py: connecting to {}:{}".format(server_ip_address,
                                               TCP_PORT))
    sock.connect((server_ip_address, TCP_PORT))

    print("run.py: sending '{}'".format(TCP_STRING))
    sock.sendall(TCP_STRING)

    string = sock.recv(len(TCP_STRING))
    print("run.py: received '{}'".format(string))

    # Polled receive.
    time.sleep(0.5)
    print("run.py: sending '{}'".format(TCP_STRING[::-1]))
    sock.sendall(TCP_STRING[::-1])

    string = sock.recv(len(TCP_STRING))
    print("run.py: received '{}'".format(string))

    assert string == TCP_STRING[::-1]

    print("run.py: closing socket")
    sock.close()


def test_tcp_write_close(server_ip_address,
                         dev,
                         timeout):
    """Perform the TCP stress test.

    """

    print("run.py: opening socket")
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Wait for the TCP test to start.
    dev.expect("listening on {}\r\n".format(TCP_PORT_WRITE_CLOSE),
               timeout=timeout)

    print("run.py: connecting to {}:{}".format(server_ip_address,
                                               TCP_PORT_WRITE_CLOSE))
    sock.connect((server_ip_address, TCP_PORT_WRITE_CLOSE))
    sock.settimeout(5)

    # Send a 1800 bytes packet and close the socket before the data
    # has been read by the peer.
    request = 533 * ' '
    print("run.py: sending {} bytes".format(len(request)))
    sock.sendall(request)
    sock.close()


def test_tcp_sizes(server_ip_address,
                   dev,
                   timeout):
    """Perform the TCP stress test.

    """

    print("run.py: opening socket")
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Wait for the TCP test to start.
    dev.expect("listening on {}\r\n".format(TCP_PORT_SIZES),
               timeout=timeout)

    # Polled listen.
    time.sleep(0.5)

    print("run.py: connecting to {}:{}".format(server_ip_address,
                                       TCP_PORT_SIZES))
    sock.connect((server_ip_address, TCP_PORT_SIZES))
    sock.settimeout(5)

    # Send a packet and wait for the echo. Test many sizes.
    for i in range(1, 5000, 128):
        request = ''.join([chr(i % 256) for i in range(i)])
        print("run.py: sending {} bytes".format(len(request)))
        sock.sendall(request)
        response = ''
        while len(response) < len(request):
            data = sock.recv(len(request) - len(response))
            print("run.py: received {} bytes".format(len(data)))
            response += data
        if request != response:
            print("run.py: sent: '{}', received: '{}'".format(request,
                                                              response))
            sys.exit(1)

    # Send a 1800 bytes packet and recieve small chunks of it.
    request = 1800 * ' '
    print("run.py: sending {} bytes".format(len(request)))
    sock.sendall(request)
    response = ''
    while len(response) < len(request):
        data = sock.recv(128)
        print("run.py: received {} bytes".format(len(data)))
        response += data
    if request != response:
        print("run.py: sent: '{}', received: '{}'".format(request,
                                                          response))
        sys.exit(1)

    print("run.py: closing socket")
    sock.close()
