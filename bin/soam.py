#!/usr/bin/env python
#

from __future__ import print_function

import sys
import struct
import argparse
import logging
import cmd
import threading
import serial

LOGGER = logging.getLogger(__name__)

SOAM_TYPE_COMMAND_REQUEST       = 1
SOAM_TYPE_COMMAND_RESPONSE_DATA = 2
SOAM_TYPE_COMMAND_RESPONSE      = 3
SOAM_TYPE_LOG_POINT             = 4


def crc_ccitt(data):
    msb = 0xff
    lsb = 0xff

    for c in data:
        x = ord(c) ^ msb
        x ^= (x >> 4)
        msb = (lsb ^ (x >> 3) ^ (x << 4)) & 255
        lsb = (x ^ (x << 5)) & 255

    return (msb << 8) + lsb


def create_packet(packet_type, payload):
    """Create a SOAM/SLIP packet.

    """

    soam_packet = struct.pack('>BH', packet_type, len(payload) + 2)
    soam_packet += payload
    soam_packet += struct.pack('>H', crc_ccitt(soam_packet))

    slip_packet = b'\xc0'

    for byte in soam_packet:
        if byte == b'\xc0':
            slip_packet += b'\xdb\xdc'
        elif byte == b'\xdb':
            slip_packet += b'\xdb\xdd'
        else:
            slip_packet += byte

    slip_packet += b'\xc0'

    return slip_packet


class TimeoutError(Exception):
    pass


class ReaderThread(threading.Thread):

    def __init__(self, client):
        super(ReaderThread, self).__init__()
        self.client = client
        self.running = True
        self.response_packet_cond = threading.Condition()
        self.response_packet = None

    def _read_packet(self):
        """Read a packet from the SOAM server.

        """

        packet = b''
        is_escaped = False

        while self.running:
            byte = self.client.serial.read(1)

            if not is_escaped:
                if byte == b'\xc0':
                    if len(packet) > 0:
                        return packet
                elif byte == b'db':
                    is_escaped = True
                else:
                    packet += byte
            else:
                if byte == b'\xdc':
                    packet += b'c0'
                elif byte == b'\xdd':
                    packet += b'db'
                else:
                    # Protocol error. Discard the packet.
                    print('Discarding:', packet)
                    packet = b''

                is_escaped = False


    def read_command_response(self, timeout=None):
        """Wait for a response packet of given type.

        """

        with self.response_packet_cond:
            if self.response_packet is None:
                self.response_packet_cond.wait(timeout)

            if self.response_packet is None:
                raise TimeoutError("no response packet received")

            packet = self.response_packet
            self.response_packet = None

        return packet[0], packet[1]

    def run(self):
        """Read packets from the soam server.

        """

        response_data = b''

        while self.running:
            try:
                packet = self._read_packet()
                if packet is None:
                    break
            except BaseException as e:
                LOGGER.info("failed to read packet with error: %s", e)
                break

            #print(packet.encode('hex'))

            if len(packet) < 5:
                continue

            # Parse the received packet.
            crc = crc_ccitt(packet[:-2])

            if crc != struct.unpack('>H', packet[-2:])[0]:
                print('Packet CRC mismatch:', packet)
                continue

            packet_type = struct.unpack('B', packet[0:1])[0]

            if packet_type == SOAM_TYPE_COMMAND_RESPONSE_DATA:
                response_data += packet[5:-2]
            elif packet_type == SOAM_TYPE_COMMAND_RESPONSE:
                code = struct.unpack('>i', packet[5:-2])[0]

                with self.response_packet_cond:
                    self.response_packet = code, response_data
                    response_data = b''
                    self.response_packet_cond.notify_all()
            elif packet_type == SOAM_TYPE_LOG_POINT:
                print(packet[3:-2], end='')
            else:
                print('Bad packet type:', packet)

    def stop(self):
        self.running = False


class SlipSerialClient(object):

    def __init__(self, serial_port, baudrate):
        self.serial = serial.Serial(serial_port,
                                    baudrate=baudrate,
                                    timeout=0.5)
        self.reader = ReaderThread(self)
        self.reader.start()

    def execute_command(self, command_with_args):
        """Execute given file system (fs) command and return a tuple of the
        command status code and output.

        """

        command = command_with_args.split(' ')[0]
        command_id = command
        command_with_args = command_with_args.replace(command, command_id, 1)
        command_with_args += b'\x00'

        packet = create_packet(SOAM_TYPE_COMMAND_REQUEST, command_with_args)

        self.serial.write(packet)

        return self.reader.read_command_response(3.0)


class CommandStatus(object):

    OK = "OK"
    ERROR = "ERROR"


def handle_errors(func):
    """Decorator that catches exceptions and prints them.

    """

    def wrapper(*args, **kwargs):
        self = args[0]

        try:
            return func(*args, **kwargs)
        except KeyboardInterrupt:
            LOGGER.info("Keyboard interrupt.")
            self.output(CommandStatus.ERROR)
        except BaseException as e:
            self.output_exception(e)
            self.output(CommandStatus.ERROR)

    # The command help needs the docstring on the wrapper function.
    wrapper.__doc__ = func.__doc__

    return wrapper


class Shell(cmd.Cmd):

    intro = "Welcome to the SOAM shell.\n\nType help or ? to list commands.\n"
    prompt = "$ "

    def __init__(self, client, stdout=None):
        if stdout is None:
            stdout = sys.stdout

        cmd.Cmd.__init__(self, stdout=stdout)

        self.client = client
        self.stdout = stdout

    def emptyline(self):
        pass

    def output_exception(self, e):
        text = "{module}.{name}: {message}".format(module=type(e).__module__,
                                                   name=type(e).__name__,
                                                   message=str(e))
        LOGGER.warning(text)
        self.output(text)

    def output(self, text="", end="\n"):
        print(text, file=self.stdout, end=end)

    @handle_errors
    def do_command(self, args):
        """Execute given command.

        """

        if not args:
            self.output("Invalid command.")
            self.output(CommandStatus.ERROR)
            return

        code, output = self.client.execute_command(args)
        self.output(output, end='')

        if code == 0:
            self.output(CommandStatus.OK)
        else:
            self.output(CommandStatus.ERROR + '({})'.format(code))

    @handle_errors
    def do_exit(self, args):
        """Exit the shell.

        """

        del args
        self.output()
        self.output()
        self.output("Bye!")
        return True

    @handle_errors
    def do_EOF(self, args):
        """Exit the shell.

        """

        return self.do_exit(args)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--port', default='/dev/ttyACM1')
    parser.add_argument('-b', '--baudrate', type=int, default=115200)
    args = parser.parse_args()

    client = SlipSerialClient(args.port, args.baudrate)
    shell = Shell(client)
    shell.cmdloop()
    client.reader.stop()

if __name__ == '__main__':
    main()
