#!/usr/bin/env python

import sys
import serial
import expect
import re
import argparse


def main():
    """Main function.

    """

    parser = argparse.ArgumentParser()
    parser.add_argument("--port", default="/dev/arduino")
    parser.add_argument("--baudrate", type=int, default=38400)
    parser.add_argument("--timeout", type=int, default=10)
    parser.add_argument("--pattern")
    parser.add_argument("--pattern-success")
    parser.add_argument("--dtr", type=int, default=0)
    parser.add_argument("--rts", type=int, default=0)
    args = parser.parse_args()

    dev_serial = serial.Serial(args.port,
                               baudrate=args.baudrate,
                               timeout=1.0)
    dev_serial.dtr = args.dtr
    dev_serial.rts = args.rts
    dev = expect.Handler(dev_serial,
                         break_conditions=[])

    status = 0

    print
    print "INFO: TIMEOUT = {}".format(args.timeout)

    try:
        print
        report = dev.expect(args.pattern, timeout=args.timeout)
        print
    except:
        print
        status = 1
    else:
        if not re.match(args.pattern_success, report):
            status = 1

    print
    sys.exit(status)


if __name__ == "__main__":
    main()
