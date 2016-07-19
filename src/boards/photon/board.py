#!/usr/bin/env python

from __future__ import print_function

import sys
import subprocess
import time
import argparse
import serial


def upload(port, binary):
    """Upload the binary to the board.

    """

    dev_serial = serial.Serial(port)

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
        "--download", binary
    ]

    print(' '.join(command))

    try:
        subprocess.check_call(command)
        return True
    except subprocess.CalledProcessError:
        return False


def subcommand_upload(args):
    """File upload subcommand.

    """

    attempt = 1

    while attempt <= 3:
        print("Uploading the software attempt {}/3.".format(attempt))

        if upload(args.port, args.binary):
            break

        attempt += 1
    else:
        print("Failed to upload {} to the Photon.".format(args.binary))
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
