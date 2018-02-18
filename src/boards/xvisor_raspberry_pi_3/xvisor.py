#!/usr/bin/env python3

from __future__ import print_function
from __future__ import unicode_literals

import sys
import argparse
import time

from serial import Serial
import expect


__version__ = '0.1'


SUCCESS = '=============================== TEST END (PASSED) =============================='

CTRL_X_Q = b'\x1bxq'


class AsciiSerial(Serial):

    def read(self, size):
        while True:
            data = super().read(size)

            try:
                return data.decode('ascii')
            except UnicodeDecodeError:
                pass

    def read_byte(self):
        return super().read(1)

    def write(self, string):
        try:
            super().write(string.encode('ascii'))
        except:
            super().write(string)


def do_upload(args):
    serial = AsciiSerial(args.serial_port,
                         baudrate=args.baudrate)
    handler = expect.Handler(serial)

    print('Entering the U-Boot... ')

    serial.write(CTRL_X_Q)
    handler.send('')
    serial.flushInput()
    handler.send('reset')
    handler.expect('Hit any key to stop autoboot:')
    handler.send('')
    handler.expect('U-Boot>')

    commands = [
        'setenv ipaddr 192.168.0.200',
        'setenv serverip 192.168.0.4',
        'tftpboot 0x200000 uvmm.bin',
        'tftpboot 0x800000 board.dtb',
        'tftpboot 0x2000000 udisk.img'
    ]

    for command in commands:
        handler.send(command)
        handler.expect('U-Boot>')

    handler.send('bootm 0x200000 0x2000000 0x800000')
    handler.expect('simba: Kicked')


def do_rerun(args):
    serial = AsciiSerial(args.serial_port,
                         baudrate=args.baudrate)
    handler = expect.Handler(serial)

    print('Restarting the Simba Xvisor guest.')

    serial.write(CTRL_X_Q)
    handler.send('')
    serial.flushInput()
    handler.expect('XVisor# ')
    time.sleep(0.1)
    handler.send('guest reset simba')
    handler.expect('XVisor# ')
    time.sleep(0.1)
    handler.send('guest kick simba')
    handler.expect('XVisor# ')
    time.sleep(0.1)
    handler.send('vserial bind simba/uart0')

    line = ''

    while line != SUCCESS:
        line = ''

        while not line.endswith('\n'):
            try:
                line += serial.read_byte().decode('ascii').strip('\r')
            except UnicodeDecodeError:
                pass

        line = line.replace('[simba/uart0] ', '').rstrip()
        print(line)


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('-d', '--debug', action='store_true')
    parser.add_argument('--version',
                        action='version',
                        version=__version__,
                        help='Print version information and exit.')
    parser.add_argument('-s', '--serial-port',
                        default='/dev/arduino',
                        help='Serial port.')
    parser.add_argument('-b', '--baudrate',
                        type=int,
                        default=115200,
                        help='Baudrate.')

    # Workaround to make the subparser required in Python 3.
    subparsers = parser.add_subparsers(title='subcommands',
                                       dest='subcommand')
    subparsers.required = True

    upload_parser = subparsers.add_parser('upload')
    upload_parser.set_defaults(func=do_upload)

    rerun_parser = subparsers.add_parser('rerun')
    rerun_parser.set_defaults(func=do_rerun)

    args = parser.parse_args()

    if args.debug:
        args.func(args)
    else:
        try:
            args.func(args)
        except BaseException as e:
            sys.exit(str(e))



if __name__ == '__main__':
    main()
