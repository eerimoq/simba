#!/usr/bin/env python

from __future__ import print_function

import sys
import argparse
import time
import re
import serial
import expect


def main():
    """Main function.

    """

    parser = argparse.ArgumentParser()
    parser.add_argument("--port", default="/dev/arduino")
    parser.add_argument("--baudrate", type=int, default=38400)
    parser.add_argument("--timeout", type=int, default=10)
    parser.add_argument("--pattern")
    parser.add_argument("--pattern-success")

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

    try:
        print()
        report = dev.expect(args.pattern, timeout=args.timeout)
        print()
    except:
        print()
        status = 1
    else:
        if not re.match(args.pattern_success, report):
            status = 1

    print()
    sys.exit(status)


if __name__ == "__main__":
    main()
