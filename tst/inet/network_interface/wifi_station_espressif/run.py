#!/usr/bin/env python

from __future__ import print_function

import argparse
import socket
import serial
import expect
import re
import sys
import time


UDP_PORT = 30303
TCP_PORT = 40404
TCP_PORT_SIZES = 40405

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
    
    # Polled receive.
    print("run.py: sending '{}' to ('{}', {})".format(UDP_STRING[::-1],
                                                      server_ip_address,
                                                      UDP_PORT))
    sock.sendto(UDP_STRING[::-1], (server_ip_address, UDP_PORT))

    string, address = sock.recvfrom(1024)
    print("run.py: received '{}' from {}".format(string, address))

    assert string == UDP_STRING[::-1]

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
            

    print("run.py: closing socket")
    sock.close()


def main():
    """Main function.

    """

    parser = argparse.ArgumentParser()

    # The default run script arguments.
    parser.add_argument("--port", default="/dev/arduino")
    parser.add_argument("--baudrate", type=int, default=38400)
    parser.add_argument("--timeout", type=int, default=10)
    parser.add_argument("--pattern")
    parser.add_argument("--pattern-success")
    parser.add_argument("--dtr", type=int, default=0)
    parser.add_argument("--rts", type=int, default=0)

    parser.add_argument('--server-ip-address',
                        default='192.168.1.103',
                        help='Server ip address.')
    args = parser.parse_args()

    dev_serial = serial.Serial(args.port,
                               baudrate=args.baudrate,
                               timeout=1.0)
    # Boot from Flash.
    dev_serial.rts = 0
    # Hold the chip in reset.
    dev_serial.dtr = 1
    time.sleep(0.5)
    # Let go of the reset.
    dev_serial.dtr = 0
    dev = expect.Handler(dev_serial,
                         break_conditions=[])

    status = 0

    print()
    print("INFO: TIMEOUT = {}".format(args.timeout))
    print()

    test_udp(args.server_ip_address,
             dev,
             args.timeout)

    test_tcp(args.server_ip_address,
             dev,
             args.timeout)

    test_tcp_sizes(args.server_ip_address,
                   dev,
                   args.timeout)

    try:
        report = dev.expect(args.pattern, timeout=args.timeout)
        print()
    except:
        print()
        status = 1
    else:
        if not re.match(args.pattern_success, report):
            status = 1

    print
    sys.exit(status)


if __name__ == '__main__':
    main()
