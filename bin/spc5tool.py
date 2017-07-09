#!/usr/bin/env python
#
# Erase, read from and write to SPC5 flash memory. Uploads a small
# application to SRAM using the BAM, which in turn accesses the flash
# memory.
#

from __future__ import print_function

import os
import sys
import argparse
import struct
import serial
import time
import binascii
import bincopy
from tqdm import tqdm


__version__ = '1.2'


# Command types.
COMMAND_TYPE_FAILED = -1
COMMAND_TYPE_PING   =  1
COMMAND_TYPE_ERASE  =  2
COMMAND_TYPE_READ   =  3
COMMAND_TYPE_WRITE  =  4

SERIAL_TIMEOUT = 3.0

READ_WRITE_CHUNK_SIZE = 1016

SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))


class CommandError(Exception):
    pass


def crc_ccitt(data):
    """Calculate a CRC of given data.

    """

    msb = 0xff
    lsb = 0xff

    for c in bytearray(data):
        x = c ^ msb
        x ^= (x >> 4)
        msb = (lsb ^ (x >> 3) ^ (x << 4)) & 255
        lsb = (x ^ (x << 5)) & 255

    return (msb << 8) + lsb


def packet_write(serial_connection, command_type, payload):
    """Write given packet to given serial connection.

    """

    header = struct.pack('>bH', command_type, len(payload))
    footer = struct.pack('>H', crc_ccitt(header + payload))

    serial_connection.write(header)
    time.sleep(0.002)
    serial_connection.write(payload + footer)


def packet_read(serial_connection):
    """Read a packet from given serial connection.

    """

    header = serial_connection.read(3)

    if len(header) != 3:
        print('error: failed to read packet header')
        return None, None

    command_type, payload_size = struct.unpack('>bH', header)

    if payload_size > 0:
        payload = serial_connection.read(payload_size)

        if len(payload) != payload_size:
            print('error: received {} bytes when expecting {}'.format(
                len(payload), payload_size))
            print('error: payload:', binascii.hexlify(payload))
            return None, None
    else:
        payload = b''

    footer = serial_connection.read(2)

    if len(footer) != 2:
        print('error: failed to read packet footer')
        return None, None

    crc = struct.unpack('>H', footer)[0]

    if crc != crc_ccitt(header + payload):
        print('error: crc mismatch of received packet')
        return None, None

    return command_type, payload


def execute_command(serial_connection, command_type, payload=None):
    """Execute given command and return the response payload.

    """

    if payload is None:
        payload = b''

    for _ in range(3):
        packet_write(serial_connection, command_type, payload)
        response_command_type, response_payload = packet_read(serial_connection)

        if response_command_type == command_type:
            return response_payload

    sys.exit('Communication failure.')


def flash_read(serial_connection, address, size, progress=None):
    """Read from flash.

    """

    left = size
    response_payload = b''

    while left > 0:
        if left > READ_WRITE_CHUNK_SIZE:
            size = READ_WRITE_CHUNK_SIZE
        else:
            size = left

        payload = struct.pack('>II', address, size)
        response_payload += execute_command(serial_connection,
                                            COMMAND_TYPE_READ,
                                            payload)
        address += size
        left -= size

        if progress:
            progress.update(size)

    return response_payload


def upload(serial_connection, mcu, baudrate, bam_baudrate, control_port):
    """Upload the spc5tool SRAM application using the BAM.

    """

    spc5tool_bin = os.path.join(SCRIPT_PATH,
                                'spc5tool',
                                'spc5tool-{}-{}-{}.bin'.format(mcu,
                                                               baudrate,
                                                               bam_baudrate))

    if control_port:
        control_port_connection = serial.Serial(control_port)

        # Set FAB high for serial boot mode.
        control_port_connection.dtr = False
        time.sleep(0.05)

        # Toggle RESET.
        control_port_connection.rts = False
        time.sleep(0.1)
        control_port_connection.rts = True
        time.sleep(0.1)
        control_port_connection.rts = False
        time.sleep(0.05)

        # Flush any received garbage.
        serial_connection.flushInput()

    with open(spc5tool_bin, 'rb') as fin:
        binary_data = fin.read()

    code_length = len(binary_data)
    code_length |= 0x80000000

    # All data to send to the BAM.
    payload = b'\xfe\xed\xfa\xce\xca\xfe\xbe\xef'
    payload += b'\x40\x00\x00\xfc'
    payload += struct.pack('>I', code_length + 4)
    payload += b'\x18\x00\xe0\x00'
    payload += binary_data

    print('Uploading {} to SRAM using the BAM.'.format(spc5tool_bin))

    for byte in tqdm(bytearray(payload), unit=' bytes'):
        byte = struct.pack('B', byte)
        serial_connection.write(byte)
        response_byte = serial_connection.read(1)

        if not response_byte:
            print()
            print("Upload failed! Timeout waiting for BAM response. Make "
                  "sure all serial cables are connected and that the board "
                  "is powered.")
            sys.exit(1)

        if response_byte != byte:
            print()
            print("Upload failed! Unexpected BAM response '{}' when "
                  "expecting '{}'.".format(binascii.hexlify(response_byte),
                                           binascii.hexlify(byte)))
            sys.exit(1)

    # Give the uploaded application 10 ms to start.
    time.sleep(0.01)

    print('Upload to SRAM complete.')


def erase(serial_connection, address, size):
    """Erase flash memory.

    """

    payload = struct.pack('>II', address, size)

    print('Erasing 0x{:08x}-0x{:08x}.'.format(address, address + size))

    execute_command(serial_connection, COMMAND_TYPE_ERASE, payload)

    print('Erase complete.')


def do_upload(args):
    serial_connection = serial.Serial(args.port,
                                      baudrate=args.bam_baudrate,
                                      timeout=SERIAL_TIMEOUT)
    upload(serial_connection,
           args.mcu,
           args.baudrate,
           args.bam_baudrate,
           args.control_port)


def do_ping(args):
    serial_connection = serial.Serial(args.port,
                                      baudrate=args.baudrate,
                                      timeout=SERIAL_TIMEOUT)

    execute_command(serial_connection, COMMAND_TYPE_PING)

    print('Ping response received.')


def do_flash_erase(args):
    serial_connection = serial.Serial(args.port,
                                      baudrate=args.baudrate,
                                      timeout=SERIAL_TIMEOUT)

    address = int(args.address, 0)
    size = int(args.size, 0)

    erase(serial_connection, address, size)


def do_flash_read(args):
    serial_connection = serial.Serial(args.port,
                                      baudrate=args.baudrate,
                                      timeout=SERIAL_TIMEOUT)
    address = int(args.address, 0)
    size = int(args.size, 0)
    left = size
    read_data = b''

    while left > 0:
        if left > READ_WRITE_CHUNK_SIZE:
            size = READ_WRITE_CHUNK_SIZE
        else:
            size = left

        payload = struct.pack('>II', address, size)
        read_data += execute_command(serial_connection,
                                     COMMAND_TYPE_READ,
                                     payload)
        address += size
        left -= size

    if args.outfile:
        with open(args.outfile, "wb") as fout:
            fout.write(read_data)
    else:
        print(read_data, end='')


def do_flash_write(args):
    serial_connection = serial.Serial(args.port,
                                      baudrate=args.baudrate,
                                      timeout=SERIAL_TIMEOUT)

    if not args.no_upload:
        serial_connection.baudrate = args.bam_baudrate
        upload(serial_connection,
               args.mcu,
               args.baudrate,
               args.bam_baudrate,
               args.control_port)
        serial_connection.baudrate = args.baudrate

    f = bincopy.BinFile()
    f.add_file(args.binfile)

    erase_segments = []
    total = 0

    for address, _, data in f.iter_segments():
        erase_segments.append((address, len(data)))

    if args.erase:
        for address, size in erase_segments:
            erase(serial_connection, address, size)

    print('Writing {} to flash.'.format(os.path.abspath(args.binfile)))

    with tqdm(total=total, unit=' bytes') as progress:
        for address, _, data in f.iter_segments():
            left = len(data)

            while left > 0:
                if left > READ_WRITE_CHUNK_SIZE:
                    size = READ_WRITE_CHUNK_SIZE
                else:
                    size = left

                payload = struct.pack('>II', address, size)
                payload += data[:size]
                data = data[size:]
                execute_command(serial_connection, COMMAND_TYPE_WRITE, payload)
                address += size
                left -= size
                progress.update(size)

    print('Write complete.')

    if args.verify:
        print('Verifying written data.')

        with tqdm(total=total, unit=' bytes') as progress:
            for address, _, data in f.iter_segments():
                left = len(data)

                while left > 0:
                    if left > READ_WRITE_CHUNK_SIZE:
                        size = READ_WRITE_CHUNK_SIZE
                    else:
                        size = left

                    payload = struct.pack('>II', address, size)
                    read_data = execute_command(serial_connection,
                                                COMMAND_TYPE_READ,
                                                payload)
                    address += size
                    left -= size

                    if bytearray(read_data) != data[:size]:
                        sys.exit('Verify failed.')

                    data = data[size:]
                    progress.update(size)

        print('Verify complete.')


def main():
    description = "Erase, read from and write to SPC5 flash memory. Uploads a " \
                  "small application (~3k) to SRAM using the BAM, which in " \
                  "turn accesses the flash memory."
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('-p', '--port', default='/dev/ttyUSB1')
    parser.add_argument('-m', '--mcu',
                        default='spc56d40l1',
                        choices=['spc56d30l1', 'spc56d40l1'])
    parser.add_argument('-b', '--baudrate',
                        type=int,
                        default=921600,
                        choices=[115200, 921600])
    parser.add_argument('-B', '--bam-baudrate',
                        type=int,
                        default=19200,
                        choices=[9600, 19200])
    parser.add_argument('-c', '--control-port')
    parser.add_argument('-d', '--debug', action='store_true')
    parser.add_argument('--version',
                        action='version',
                        version=__version__,
                        help='Print version information and exit.')

    subparsers = parser.add_subparsers()

    upload_parser = subparsers.add_parser('upload')
    upload_parser.set_defaults(func=do_upload)

    ping_parser = subparsers.add_parser('ping')
    ping_parser.set_defaults(func=do_ping)

    flash_erase_parser = subparsers.add_parser('flash_erase')
    flash_erase_parser.add_argument('address')
    flash_erase_parser.add_argument('size')
    flash_erase_parser.set_defaults(func=do_flash_erase)

    flash_read_parser = subparsers.add_parser('flash_read')
    flash_read_parser.add_argument('-o', '--outfile')
    flash_read_parser.add_argument('address')
    flash_read_parser.add_argument('size')
    flash_read_parser.set_defaults(func=do_flash_read)

    flash_write_parser = subparsers.add_parser('flash_write')
    flash_write_parser.add_argument('-u', '--no-upload', action='store_true')
    flash_write_parser.add_argument('-e', '--erase', action='store_true')
    flash_write_parser.add_argument('-v', '--verify', action='store_true')
    flash_write_parser.add_argument('binfile')
    flash_write_parser.set_defaults(func=do_flash_write)

    args = parser.parse_args()

    if args.debug:
        args.func(args)
    else:
        try:
            args.func(args)
        except BaseException as e:
            sys.exit(str(e))


if __name__ == "__main__":
    main()
