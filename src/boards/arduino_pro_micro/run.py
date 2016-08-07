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
    parser.add_argument("--timeout", type=int, default=10)
    parser.add_argument("--pattern")
    parser.add_argument("--pattern-success")

    args = parser.parse_args()
    
    dev_serial = serial.Serial(args.port, timeout=3.0)
    dev = expect.Handler(dev_serial,
                         break_conditions=[])

    # Wait for the board to start.
    sys.stdout.write("Waiting for the serial port '{}' to show up... ".format(
        args.port))
    attempt = 1
    while attempt < 20:
        try:
            serial.Serial(args.port, baudrate=38400).close()
            print("done.")
            break
        except:
            time.sleep(0.5)
            attempt += 1
    else:
        print("failed.")
        sys.exit(1)

    status = 0

    print("INFO: TIMEOUT = {}".format(args.timeout))
    print()

    try:
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
