#!/usr/bin/env python

from __future__ import print_function

import serial
import subprocess
import time
import argparse


def subcommand_upload(args):
    """Upload the binary to the board.

    """

    dev_serial = serial.Serial("/dev/" + args.port)

    # 1. Hold down the RESET and SETUP buttons.
    dev_serial.dtr = 1
    dev_serial.rts = 1

    # 2. Release only the RESET button, while holding down the SETUP
    #    button.
    dev_serial.rts = 0

    # 3. Wait for the LED to start flashing yellow (it will flash
    #    magenta first).
    time.sleep(4.0)

    # 4. Release the SETUP button.
    dev_serial.dtr = 0

    time.sleep(1.0)

    # 5. Upload the software,
    command = [
        "dfu-util",
        "--device", "2b04:d006",
        "--alt", "0",
        "--dfuse-address", "0x08020000:leave",
        "--download", args.binary
    ]

    print(' '.join(command))

    subprocess.check_call(command)


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
