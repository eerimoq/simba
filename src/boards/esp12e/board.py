#!/usr/bin/env python

import serial
import subprocess
import argparse
import os
import time


def subcommand_upload(args):
    """Upload the binary to the board.

    """

    esptool_path = os.path.join(os.environ["SIMBA_ROOT"],
                                "3pp",
                                "esptool",
                                "esptool.py")
    bootloader_path = os.path.join(os.environ["SIMBA_ROOT"],
                                   "3pp",
                                   "ESP8266_RTOS_SDK",
                                   "bin",
                                   "boot_v1.4.bin")

    # Set GPIO0 to low to boot into flashing mode, then reset the
    # board.
    dev_serial = serial.Serial(args.port)
    dev_serial.rts = 1
    dev_serial.dtr = 0
    time.sleep(0.2)
    dev_serial.dtr = 1
    time.sleep(3)
    subprocess.check_call([
	esptool_path,
        "--port", args.port,
        "--baud", "460800",
        "write_flash",
        "0x00000", bootloader_path,
        "0x01000", args.binary
    ])


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
