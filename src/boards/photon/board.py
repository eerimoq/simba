#!/usr/bin/env python

import serial
import subprocess
import time
import argparse


def subcommand_upload(args):
    """Upload the binary to the board.

    """

    # 1. Hold down the RESET and SETUP buttons.
    time.sleep(0.1)

    # 2. Release only the RESET button, while holding down the SETUP
    #    button.

    # 3. Wait for the LED to start flashing yellow (it will flash
    #    magenta first).
    time.sleep(3.0)

    # 4. Release the SETUP button.

    # 5. Upload the software,
    command = [
        "dfu-util",
        "--device", "2b04:d006",
        "--alt", "0",
        "--dfuse-address", "0x08020000:leave",
        "--download", args.binary
    ]
    print ' '.join(command)
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
