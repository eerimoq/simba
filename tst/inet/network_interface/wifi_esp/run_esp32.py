#!/usr/bin/env python

from __future__ import print_function

import argparse
import serial
import expect
import re
import sys
import test


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
                        default='192.168.0.7',
                        help='Server ip address.')
    args = parser.parse_args()

    dev_serial = serial.Serial(args.port,
                               baudrate=args.baudrate,
                               timeout=1.0)
    dev_serial.dtr = args.dtr
    dev_serial.rts = args.rts
    dev = expect.Handler(dev_serial,
                         break_conditions=[])

    status = 0

    print()
    print("INFO: TIMEOUT = {}".format(args.timeout))
    print()

    try:
        test.test_udp(args.server_ip_address,
                      dev,
                      args.timeout)

        test.test_tcp(args.server_ip_address,
                      dev,
                      args.timeout)

        test.test_tcp_write_close(args.server_ip_address,
                                  dev,
                                  args.timeout)

        test.test_tcp_sizes(args.server_ip_address,
                            dev,
                            args.timeout)
    finally:
        print(dev.iostream.read(dev.iostream.in_waiting))

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
