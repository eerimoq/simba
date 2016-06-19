#!/usr/bin/env python

import serial
import subprocess
import time
import argparse


def _upload(args):
    """Try to upload the binary to the board.

    """

    print ("Setting /dev/{} to 1200 baud and setting DTR to "
           "reset the board.".format(args.port))
    ser = serial.Serial("/dev/" + args.port, baudrate=1200)
    ser.setDTR(1)
    ser.close()
    time.sleep(0.4)
    command = [
        "bossac",
        "--port=" + args.port,
        "-e",
        "-w",
        "-b",
        "-R", args.binary
    ]
    print ' '.join(command)
    subprocess.check_call(command)

def subcommand_upload(args):
    """Upload the binary to the board.

    """

    try:
        _upload(args)
    except:
        try:
            # /dev/arduino cannot be opened unless a COM-port is
            # opened first
            subprocess.check_call([
                "bossac",
                "--port=" + args.port,
                "-U", "false",
                "-e",
                "-w",
                "-b", args.binary
            ])
        except:
            _upload(args)


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
