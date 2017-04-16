#!/usr/bin/env python3
#

from __future__ import print_function

import sys
import argparse
from socket_device import SocketDevice


def monitor(device_type, device_name, address, port):
    """Monitor given device.

    """

    device = SocketDevice(device_type, device_name, address, port)
    device.start()

    while True:
        byte = device.read(1)

        if not byte:
            print('Connection closed.')
            break

        try:
            print(byte.decode('ascii'), end='')
        except UnicodeDecodeError:
            print(byte)


def do_pin(args):
    monitor('pin', args.device, args.address, args.port)


def do_uart(args):
    monitor('uart', args.device, args.address, args.port)


def do_pwm(args):
    monitor('pwm', args.device, args.address, args.port)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--debug', action='store_true')
    parser.add_argument('-a', '--address',
                        default='localhost',
                        help='TCP address to connect to.')
    parser.add_argument('-p', '--port',
                        type=int,
                        default=47000,
                        help='TCP port to connect to.')

    subparsers = parser.add_subparsers()

    pin_parser = subparsers.add_parser('pin')
    pin_parser.set_defaults(func=do_pin)

    pwm_parser = subparsers.add_parser('pwm')
    pwm_parser.set_defaults(func=do_pwm)

    uart_parser = subparsers.add_parser('uart')
    uart_parser.set_defaults(func=do_uart)

    parser.add_argument('device', help='UART device to monitor.')

    args = parser.parse_args()

    if not args.debug:
        try:
            args.func(args)
        except BaseException as e:
            sys.exit(str(e))
    else:
        args.func(args)


if __name__ == '__main__':
    main()
