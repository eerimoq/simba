#!/usr/bin/env python

import serial
import subprocess
import time
import argparse
import sys


def _upload(args):
    """Try to upload the binary to the board.

    """

    print ("Setting {} to 1200 baud and setting DTR to "
           "reset the board.".format(args.port))
    ser = serial.Serial(args.port, baudrate=1200)
    ser.setDTR(False)
    ser.close()
    time.sleep(1)

    command = [
        "avrdude",
        "-p", "atmega32u4",
        "-P", args.port,
        "-c", "avr109",
        "-V",
        "-b", "57600",
        "-D",
        "-U", "flash:w:" + args.binary + ":i"
    ]
    print ' '.join(command)
    subprocess.check_call(command)
    time.sleep(1)

    # Wait for the board to start.
    sys.stdout.write("Waiting for the serial port '{}' to show up... ".format(
        args.port))
    attempt = 1
    while attempt < 20:
        try:
            serial.Serial(args.port, baudrate=38400).close()
            print "done."
            break
        except:
            time.sleep(0.5)
            attempt += 1
    else:
        print "failed."
        print "Uploaded software does not setup the serial port correctly."
        sys.exit(1)

def subcommand_upload(args):
    """Upload the binary to the board.

    """

    try:
        _upload(args)
    except:
        _upload(args)


def main():
    """Main function.

    """

    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers()

    upload = subparsers.add_parser('upload')
    upload.add_argument("--port", default="/dev/arduino")
    upload.add_argument("binary")
    upload.set_defaults(func=subcommand_upload)

    args = parser.parse_args()

    args.func(args)


if __name__ == "__main__":
    main()
