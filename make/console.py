#!/usr/bin/env python

import sys
import threading
import argparse
import readchar
import serial
import time

def serial_reader_main(dev_serial):
    """Serial reader main function.

    """

    while True:
        sys.stdout.write(dev_serial.read(1))
        sys.stdout.flush()


def main():
    """The main function.

    """

    parser = argparse.ArgumentParser()
    parser.add_argument("--port", default="/dev/arduino")
    parser.add_argument("--reset-type", type=int, default=-1)
    parser.add_argument("--baudrate", "-b",
                        type=int,
                        required=True)
    args = parser.parse_args()

    print
    print "Press CTRL-C to exit the console."
    print

    dev_serial = serial.Serial(args.port, baudrate=args.baudrate)

    if args.reset_type == 0:
        dev_serial.rts = 0
        dev_serial.dtr = 1
        time.sleep(0.5)
        # Let go of the reset.
        dev_serial.dtr = 0

    # Start the serial reader thread.
    serial_reader_thread = threading.Thread(target=serial_reader_main,
                                            args=(dev_serial, ))
    serial_reader_thread.setDaemon(True)
    serial_reader_thread.start()

    # Read user input.
    while True:
        key = readchar.readkey()

        # Break on CTRL-C.
        if key == readchar.key.CTRL_C:
            break

        if key == readchar.key.ENTER:
            dev_serial.write('\r\n')
        else:
            dev_serial.write(key)

    print


if __name__ == '__main__':
    main()
