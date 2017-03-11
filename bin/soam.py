#!/usr/bin/env python
#

from __future__ import print_function

import sys
import struct
import argparse
import cmd
import threading
import serial
import hashlib

# SOAM protocol definitions.
SOAM_TYPE_STDOUT                       = 1
SOAM_TYPE_LOG_POINT                    = 2
SOAM_TYPE_COMMAND_REQUEST              = 3
SOAM_TYPE_COMMAND_RESPONSE_DATA_PRINTF = 4
SOAM_TYPE_COMMAND_RESPONSE_DATA_BINARY = 5
SOAM_TYPE_COMMAND_RESPONSE             = 6
SOAM_TYPE_DATABASE_ID_REQUEST          = 7
SOAM_TYPE_DATABASE_ID_RESPONSE         = 8
SOAM_TYPE_INVALID_TYPE                 = 9

SOAM_SEGMENT_SIZE_MIN = 6

SOAM_SEGMENT_FLAGS_CONSECUTIVE = (1 << 1)
SOAM_SEGMENT_FLAGS_LAST        = (1 << 0)


class CommandNotFoundError(Exception):
    pass


def crc_ccitt(data):
    msb = 0xff
    lsb = 0xff

    for c in data:
        x = ord(c) ^ msb
        x ^= (x >> 4)
        msb = (lsb ^ (x >> 3) ^ (x << 4)) & 255
        lsb = (x ^ (x << 5)) & 255

    return (msb << 8) + lsb


class TimeoutError(Exception):
    pass


class Database(object):

    def __init__(self, database):
        self.formats = {}
        self.commands = {}

        with open(database) as fin:
            for line in fin.readlines():
                # Ignore comments.
                if line.startswith('#'):
                    continue

                kind, identity, string = line.strip().split(' ', 2)
                identity = int(identity, 0)
                string = string[1:-1]
                string = string.replace('\\n', '\n')
                string = string.replace('\\r', '\r')
                string = string.replace('\\t', '\t')
                string = string.replace('\\v', '\v')
                string = string.replace('\\\\', '\\')

                if kind == 'FMT:':
                    self.formats[identity] = string
                elif kind == 'CMD:':
                    self.commands[string] = identity
                else:
                    sys.exit('{}: bad kind'.format(kind))


class ReaderThread(threading.Thread):

    def __init__(self, client):
        super(ReaderThread, self).__init__()
        self.client = client
        self.running = True
        self.response_packet_cond = threading.Condition()
        self.response_packet = None

    def read_soam_segment(self):
        """Read a packet from the SOAM server.

        This method is overridden by subclasses.

        """

        raise NotImplementedError()

    def read_response(self, timeout=None):
        """Wait for a response packet of given type.

        """

        with self.response_packet_cond:
            if self.response_packet is None:
                self.response_packet_cond.wait(timeout)

            if self.response_packet is None:
                raise TimeoutError("no response packet received")

            packet = self.response_packet
            self.response_packet = None

        return packet

    def run(self):
        """Read packets from the soam server.

        """

        segments = None
        response_data = []
        segment_index = None

        while self.running:
            try:
                segment = self.read_soam_segment()
            except BaseException:
                print("warning: failed to read segment")
                continue

            #print(segment.encode('hex'))

            if len(segment) < SOAM_SEGMENT_SIZE_MIN:
                continue

            # Parse the received packet.
            crc = crc_ccitt(segment[:-2])

            if crc != struct.unpack('>H', segment[-2:])[0]:
                print('Segment CRC mismatch:', segment)
                segments = None
                continue

            segment_type_flags, index = struct.unpack('BB', segment[0:2])
            segment_type = (segment_type_flags >> 4)
            flags = (segment_type_flags & 0xf)
            payload = segment[4:-2]

            if segment_index is None:
                segment_index = index + 1
            elif index == segment_index:
                segment_index += 1
                segment_index &= 0xff
            else:
                print('Bad segment index.')
                segment_index = index + 1
                segments = None
                continue

            # Receive all segments.
            if segments == None:
                # Waiting for a first/last segment.
                if flags & SOAM_SEGMENT_FLAGS_CONSECUTIVE:
                    continue

                segments = [payload]
            else:
                if flags == 0:
                    segments = [payload]
                else:
                    segments += [payload]

            if (flags & SOAM_SEGMENT_FLAGS_LAST) == 0:
                continue

            packet_type = segment_type
            packet = ''.join(segments)

            # Decode the reassembled packet.
            if packet_type == SOAM_TYPE_STDOUT:
                print(packet, end='')
            elif packet_type == SOAM_TYPE_LOG_POINT:
                # Format the log point.
                header, data = packet.split(': ', 1)
                identity = struct.unpack('>H', data[0:2])[0]
                fmt = self.client.database.formats[identity]
                formatted_log_point = (header
                                       + ': '
                                       + fmt.format(*data[2:].split('\x1f')))
                print(formatted_log_point, end='')
            elif packet_type in [SOAM_TYPE_COMMAND_RESPONSE_DATA_PRINTF,
                                 SOAM_TYPE_COMMAND_RESPONSE_DATA_BINARY]:
                response_data.append((packet_type, packet))
            elif packet_type == SOAM_TYPE_COMMAND_RESPONSE:
                code = struct.unpack('>i', packet)[0]

                with self.response_packet_cond:
                    self.response_packet = code, response_data
                    response_data = []
                    self.response_packet_cond.notify_all()
            elif packet_type == SOAM_TYPE_DATABASE_ID_RESPONSE:
                with self.response_packet_cond:
                    self.response_packet = packet
                    self.response_packet_cond.notify_all()
            elif packet_type == SOAM_TYPE_INVALID_TYPE:
                print('warning: "invalid type" packet received')
            else:
                print('warning: {}: bad packet type', packet_type)

            segments = None

    def stop(self):
        self.running = False


class Client(object):

    def __init__(self, database, reader):
        self.database = Database(database)
        self.packet_index = 1
        self.reader = reader
        self.reader.setDaemon(True)
        self.reader.start()

        try:
            device_database_id = self.get_database_id_from_device()
        except BaseException:
            sys.exit('error: failed to read the database id from the device')

        with open(database, 'rb') as fin:
            database_id = hashlib.md5(fin.read()).hexdigest()

        if device_database_id != database_id:
            sys.exit('error: database id mismatch ({} != {})'.format(
                device_database_id,
                database_id))

    def write_soam_segment(self, segment):
        """Write given packet to the server.

        This method is overridden by subclasses.

        """

        raise NotImplementedError()

    def create_soam_segment(self, packet_type, payload):
        """Create a SOAM packet.

        """

        packet = struct.pack('>BBH',
                             packet_type << 4,
                             self.packet_index,
                             len(payload) + 2)
        packet += payload
        packet += struct.pack('>H', crc_ccitt(packet))

        self.packet_index += 1
        self.packet_index &= 0xff

        return packet

    def get_database_id_from_device(self):
        """Get the database id from the device.

        """

        segment = self.create_soam_segment(SOAM_TYPE_DATABASE_ID_REQUEST, b'')
        self.write_soam_segment(segment)

        return self.reader.read_response(3.0)

    def execute_command(self, command_with_args):
        """Execute given file system (fs) command and return a tuple of the
        command status code and output.

        """

        if command_with_args[0] != '/':
            command_with_args = '/' + command_with_args
        command = command_with_args.split(' ')[0]

        try:
            command_id = struct.pack('>H', self.database.commands[command])
        except KeyError:
            raise CommandNotFoundError(
                "{}: command not found in string database".format(command))

        command_with_args = command_with_args.replace(command, command_id, 1)
        command_with_args += b'\x00'

        segment = self.create_soam_segment(SOAM_TYPE_COMMAND_REQUEST,
                                           command_with_args)
        self.write_soam_segment(segment)

        code, response_data_list = self.reader.read_response(3.0)
        formatted_response_data = ''

        for packet_type, response_data in response_data_list:
            if packet_type == SOAM_TYPE_COMMAND_RESPONSE_DATA_PRINTF:
                identity = struct.unpack('>H', response_data[0:2])[0]
                try:
                    fmt = self.database.formats[identity]
                    formatted_response_data += fmt.format(
                        *response_data[2:].split('\x1f'))
                except KeyError:
                    formatted_response_data += response_data
            else:
                formatted_response_data += response_data

        return code, formatted_response_data


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
            print("Keyboard interrupt.")
            self.output(CommandStatus.ERROR)
        except CommandNotFoundError as e:
            print(e)
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
    identchars = cmd.Cmd.identchars + '/'

    def __init__(self, client, stdout=None):
        if stdout is None:
            stdout = sys.stdout
        cmd.Cmd.__init__(self, stdout=stdout)

        for command in client.database.commands:
            parts = command[1:].split('/')
            command = parts[0]

            if len(parts) > 1:
                command += '/'

            setattr(self.__class__, 'do_' + command, self.default)

        self.client = client
        self.stdout = stdout
        self.line = None

    def emptyline(self):
        pass

    @handle_errors
    def default(self, line):
        code, output = self.client.execute_command(self.line)
        self.output(output, end='')

        if code == 0:
            self.output(CommandStatus.OK)
        else:
            self.output(CommandStatus.ERROR + '({})'.format(code))

    def completedefault(self, *args):
        _, line, begidx, _ = args
        alternatives = []

        for command in self.client.database.commands:
            if command[1:].startswith(line):
                alternatives.append(command)

        completions = []

        for alternative in alternatives:
            parts = alternative[begidx + 1:].split('/')
            completion = parts[0]

            # Directories.
            if len(parts) > 1:
                completion += '/'
            else:
                completion += ' '

            completions.append(completion)

        return completions

    def output_exception(self, e):
        text = "{module}.{name}: {message}".format(module=type(e).__module__,
                                                   name=type(e).__name__,
                                                   message=str(e))
        print(text)
        self.output(text)

    def output(self, text="", end="\n"):
        print(text, file=self.stdout, end=end)

    def precmd(self, line):
        self.line = line
        return line

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


class SlipSerialReaderThread(ReaderThread):

    def __init__(self, client):
        super(SlipSerialReaderThread, self).__init__(client)

    def read_soam_segment(self):
        """Read an SLIP packet.

        """

        packet = b''
        is_escaped = False

        while self.running:
            byte = self.client.serial.read(1)

            # Print stored packet on timeout. Likely non-framed data,
            # for example from a panic.
            if not byte:
                if len(packet) > 0:
                    print('warning: {}: serial read failure, discarding '
                          'packet'.format(packet))
                    packet = b''

                continue

            if not is_escaped:
                if byte == b'\xc0':
                    if len(packet) > 0:
                        return packet
                elif byte == b'\xdb':
                    is_escaped = True
                else:
                    packet += byte
            else:
                if byte == b'\xdc':
                    packet += b'\xc0'
                elif byte == b'\xdd':
                    packet += b'\xdb'
                else:
                    # Protocol error. Discard the packet.
                    print('warning: {}: discarding packet'.format(packet))
                    packet = b''

                is_escaped = False


class SlipSerialClient(Client):

    def __init__(self, serial_port, baudrate, database):
        self.serial = serial.Serial(serial_port,
                                    baudrate=baudrate,
                                    timeout=0.5)
        super(SlipSerialClient, self).__init__(database,
                                               SlipSerialReaderThread(self))

    def write_soam_segment(self, segment):
        """Wrap given packet in SLIP and write it to the serial connection.

        """

        packet = b'\xc0'

        for byte in segment:
            if byte == b'\xc0':
                packet += b'\xdb\xdc'
            elif byte == b'\xdb':
                packet += b'\xdb\xdd'
            else:
                packet += byte

        packet += b'\xc0'

        self.serial.write(packet)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--port', default='/dev/ttyACM1')
    parser.add_argument('-b', '--baudrate', type=int, default=115200)
    parser.add_argument('database')
    args = parser.parse_args()

    client = SlipSerialClient(args.port, args.baudrate, args.database)
    shell = Shell(client)
    shell.cmdloop()
    client.reader.stop()


if __name__ == '__main__':
    main()
