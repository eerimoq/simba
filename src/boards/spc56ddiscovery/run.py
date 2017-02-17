#!/usr/bin/env python

import sys
import serial
import expect
import re
import argparse
import time


def main():
    """Main function.

    """

    parser = argparse.ArgumentParser()
    parser.add_argument("--port", default="/dev/arduino")
    parser.add_argument("--control-port")
    parser.add_argument("--baudrate", type=int, default=115200)
    parser.add_argument("--timeout", type=int, default=10)
    parser.add_argument("--pattern")
    parser.add_argument("--pattern-success")
    args = parser.parse_args()

    serial_connection = serial.Serial(args.port,
                                      baudrate=args.baudrate,
                                      timeout=1.0)

    # Set FAB low to boot from flash.
    if args.control_port:
        control_port_connection = serial.Serial(args.control_port)

        # Set RESET high.
        control_port_connection.rts = False
        
        control_port_connection.dtr = True
        time.sleep(0.01)

        # Toggle RESET.
        control_port_connection.rts = True
        time.sleep(0.005)
        control_port_connection.rts = False
        time.sleep(0.01)

    dev = expect.Handler(serial_connection,
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
