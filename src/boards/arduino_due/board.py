#!/usr/bin/env python3

import serial
import subprocess
import time
import argparse


def _upload(port, binary, bossac_port, unlock):
    """Try to upload the binary to the board.

    """

    print("Setting {} to 1200 baud and setting DTR to "
          "reset the board.".format(port))
    ser = serial.Serial(port, baudrate=1200)
    ser.setDTR(1)
    ser.close()
    time.sleep(0.4)
    command = [
        "bossac",
        "--port=" + bossac_port,
        "-e",
        "-w",
        "-b",
        "-R", binary
    ]
    command += unlock
    print(' '.join(command))
    subprocess.check_call(command)


def subcommand_upload(args):
    """Upload the binary to the board.

    """

    bossac_port = args.port.replace("/dev/", "")
    unlock = []

    if args.unlock:
        unlock = ["-u"]

    try:
        _upload(args.port, args.binary, bossac_port, unlock)
    except:
        try:
            # /dev/arduino cannot be opened unless a COM-port is
            # opened first
            subprocess.check_call([
                "bossac",
                "--port=" + bossac_port,
                "-U", "false",
                "-e",
                "-w",
                "-b", args.binary
                ] + unlock)
        except:
            _upload(args.port, args.binary, bossac_port, unlock)


def main():
    """Main function.

    """

    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers()

    upload = subparsers.add_parser('upload')
    upload.add_argument("--port", default="/dev/arduino")
    upload.add_argument("--unlock", action='store_true')
    upload.add_argument("binary")
    upload.set_defaults(func=subcommand_upload)

    args = parser.parse_args()

    args.func(args)


if __name__ == "__main__":
    main()
