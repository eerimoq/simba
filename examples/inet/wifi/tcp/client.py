#!/usr/bin/env python

from __future__ import print_function

import argparse
import socket
import time

TCP_PORT = 40404

TCP_STRING = "hello tcp\x00"


def tcp_test(server_ip_address):
    """Perform the TCP test.

    """

    print("opening socket")
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    print("connecting to {}:{}".format(server_ip_address,
                                       TCP_PORT))
    sock.connect((server_ip_address, TCP_PORT))

    print("sending '{}'".format(TCP_STRING))
    sock.sendall(TCP_STRING)

    string = sock.recv(len(TCP_STRING))
    print("received '{}'".format(string))

    assert string == TCP_STRING

    print("closing socket")
    sock.close()


def main():
    """Main function.

    """

    parser = argparse.ArgumentParser()

    parser.add_argument('--server-ip-address',
                        default='192.168.1.103',
                        help='Server ip address.')
    args = parser.parse_args()

    time.sleep(1)
    tcp_test(args.server_ip_address)


if __name__ == '__main__':
    main()
