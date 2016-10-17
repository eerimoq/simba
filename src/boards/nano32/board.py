#!/usr/bin/env python

from __future__ import print_function

import sys
import subprocess
import argparse
import os
import serial
import time


def subcommand_upload(args):
    """Upload the binary to the board.

    """

    esptool_path = os.path.join(os.environ["SIMBA_ROOT"],
                                "3pp",
                                "esptool",
                                "esptool")

    attempt = 1
    attempts_max = 5

    while attempt <= attempts_max:
        print("Attempt {}/{}.".format(attempt, attempts_max))

        try:
            port = serial.Serial(args.port)
            port.setRTS(True)
            port.setDTR(True)
            time.sleep(0.1)
            port.setDTR(False)

            subprocess.check_call([
                esptool_path,
                "-cd", "none",
                "-cb", "230400",
                "-cp", args.port,
                "-cf", args.binary
            ])

            break
        except:
            attempt += 1
    else:
        sys.exit(1)


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
