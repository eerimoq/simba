#!/usr/bin/env python

import sys
import serial
import subprocess
import time
import argparse


def subcommand_upload(args):
    """Upload the binary to the board.

    """

    attempt = 1
    attempts_max = 5

    while attempt <= attempts_max:
        print("Uploading the software attempt {}/{}.".format(
            attempt,
            attempts_max))
        print("Resetting the board by pulling the reset pin "
              "low for 0.1 seconds.")
        dev_serial = serial.Serial(args.port)
        dev_serial.dtr = 1
        time.sleep(0.1)
        dev_serial.dtr = 0
        dev_serial.close()
        command = [
            "st-flash",
            "write", args.binary, "0x08000000"
        ]
        print ' '.join(command)
        try:
            subprocess.check_call(command)
            break
        except:
            attempt += 1
    else:
        print("Failed to upload the software.")
        sys.exit(1)

def main():
    """Main function.

    """

    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers()

    upload = subparsers.add_parser('upload')
    upload.add_argument("--port", default="arduino")
    upload.add_argument("binary")
    upload.set_defaults(func=subcommand_upload)

    args = parser.parse_args()

    args.func(args)


if __name__ == "__main__":
    main()
