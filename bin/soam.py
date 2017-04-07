#!/usr/bin/env python3
#

from __future__ import print_function

import sys
import struct
import argparse
import cmd
import threading
import serial
import hashlib
import lzma
import traceback

try:
    from StringIO import StringIO
except ImportError:
    from io import StringIO

# SOAM protocol definitions.
SOAM_TYPE_STDOUT_PRINTF                = 1
SOAM_TYPE_STDOUT_BINARY                = 2
SOAM_TYPE_LOG_POINT                    = 3
SOAM_TYPE_COMMAND_REQUEST              = 4
SOAM_TYPE_COMMAND_RESPONSE_DATA_PRINTF = 5
SOAM_TYPE_COMMAND_RESPONSE_DATA_BINARY = 6
SOAM_TYPE_COMMAND_RESPONSE             = 7
SOAM_TYPE_DATABASE_ID_REQUEST          = 8
SOAM_TYPE_DATABASE_ID_RESPONSE         = 9
SOAM_TYPE_DATABASE_REQUEST             = 10
SOAM_TYPE_DATABASE_RESPONSE            = 11
SOAM_TYPE_INVALID_TYPE                 = 15

SOAM_SEGMENT_SIZE_MIN = 6

SOAM_SEGMENT_FLAGS_CONSECUTIVE = (1 << 1)
SOAM_SEGMENT_FLAGS_LAST        = (1 << 0)


class CommandNotFoundError(Exception):
    pass


def crc_ccitt(data):
    msb = 0xff
    lsb = 0xff

    for c in bytearray(data):
        x = c ^ msb
        x ^= (x >> 4)
        msb = (lsb ^ (x >> 3) ^ (x << 4)) & 255
        lsb = (x ^ (x << 5)) & 255

    return (msb << 8) + lsb


def format_printf(database, packet):
    """Format given printf.

    """

    identity = struct.unpack('>H', packet[0:2])[0]

    try:
        fmt = database.formats[identity]
        args = packet[2:].decode('ascii').split('\x1f')
        formatted_string = fmt.format(*args)
    except KeyError:
        try:
            formatted_string = packet.decode('ascii')
        except UnicodeDecodeError:
            formatted_string = packet

    return formatted_string


class TimeoutError(Exception):
    pass


class Database(object):

    def __init__(self):
        self.formats = {}
        self.commands = {}
        self.command_id_to_string = {}

    def set_database(self, database):
        """Set the database.

        """

        for line in database.readlines():
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
                self.command_id_to_string[identity] = string
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
            packet = b''.join(segments)

            # Decode the reassembled packet.
            if packet_type == SOAM_TYPE_STDOUT_PRINTF:
                formatted_string = format_printf(self.client.database, packet)
                print(formatted_string, end='')
            elif packet_type == SOAM_TYPE_STDOUT_BINARY:
                print(packet, end='')
            elif packet_type == SOAM_TYPE_LOG_POINT:
                # Format the log point.
                header, data = packet.split(b': ', 1)
                identity = struct.unpack('>H', data[0:2])[0]

                try:
                    fmt = self.client.database.formats[identity]
                    args = data[2:].decode('ascii').split('\x1f')
                    formatted_log_point = (header.decode('ascii')
                                           + ': '
                                           + fmt.format(*args))
                except KeyError:
                    try:
                        formatted_log_point = packet.decode('ascii')
                    except UnicodeDecodeError:
                        formatted_log_point = packet

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
            elif packet_type in [SOAM_TYPE_DATABASE_ID_RESPONSE,
                                 SOAM_TYPE_DATABASE_RESPONSE]:
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
        self.database = Database()
        self.packet_index = 1
        self.reader = reader
        self.reader.setDaemon(True)
        self.reader.start()

        if database:
            with open(database, 'rb') as fin:
                database = fin.read()
        else:
            # Get the database from the device if not given.
            try:
                print('Reading database from device... ', end='', flush=True)
                database_compressed = self.get_database_from_device()
                print('done.', flush=True)
            except BaseException:
                print('failed. ', flush=True)
                sys.exit('error: failed to read the database from the device')

            database = lzma.decompress(database_compressed)

        self.database.set_database(StringIO(database.decode('ascii')))

        try:
            print('Reading database id from device... ', end='', flush=True)
            device_database_id = self.get_database_id_from_device()
            print('done.', flush=True)
        except BaseException:
            print('failed. ', flush=True)
            sys.exit('error: failed to read the database id from the device')

        print('Comparing read and calculated database ids... ', end='')

        database_id = hashlib.md5(database).hexdigest().encode('ascii')

        if device_database_id != database_id:
            print('failed. ', flush=True)
            sys.exit('error: database id mismatch ({} != {})'.format(
                device_database_id,
                database_id))

        print('done.', flush=True)

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

    def get_database_from_device(self):
        """Get the database from the device.

        """

        segment = self.create_soam_segment(SOAM_TYPE_DATABASE_REQUEST, b'')
        self.write_soam_segment(segment)

        return self.reader.read_response(10.0)

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

        command_with_args = command_with_args.encode('ascii')
        command_with_args = command_with_args.replace(command.encode('ascii'),
                                                      command_id,
                                                      1)
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
                    if code == -1003:
                        command_id = struct.unpack('>H', response_data[2:4])[0]
                        args = [self.database.command_id_to_string[command_id]]
                    else:
                        args = response_data[2:].decode('ascii').split('\x1f')
                    formatted_response_data += fmt.format(*args)
                except KeyError:
                    formatted_response_data += response_data.decode('ascii')
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

    intro = "\nWelcome to the SOAM shell.\n\nType help or ? to list commands.\n"
    prompt = "$ "
    identchars = cmd.Cmd.identchars + '/'

    def __init__(self, client, stdout=None, debug=False):
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
        self.debug = debug
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
        if self.debug:
            for entry in traceback.format_exception(type(e), e, e.__traceback__):
                for line in entry.splitlines():
                    self.output(line.rstrip())
        else:
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

        for byte in bytearray(segment):
            if byte == 0xc0:
                packet += b'\xdb\xdc'
            elif byte == 0xdb:
                packet += b'\xdb\xdd'
            else:
                packet += struct.pack('B', byte)

        packet += b'\xc0'

        self.serial.write(packet)


def decode_unframed_text_stream(stream, database):
    """Decode given stream of unframed SOAM data.

    """

    # Setup the database.
    db = Database()

    with open(database, 'rb') as fin:
        database = fin.read()

    db.set_database(StringIO(database.decode('ascii')))

    # Split on SOAM packet IDs.
    entry = stream.read(2)

    while True:
        byte = stream.read(1)

        if not byte:
            formatted_string = format_printf(db, entry)
            print(formatted_string, end='')
            break

        # First byte of an id found?
        if struct.unpack('>B', byte)[0] > 127:
            formatted_string = format_printf(db, entry)
            print(formatted_string, end='')
            entry = (byte + stream.read(1))
        else:
            entry += byte


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--port', help='Serial port.')
    parser.add_argument('-b', '--baudrate', type=int, default=115200)
    parser.add_argument('-d', '--debug', action='store_true')
    parser.add_argument('-u', '--unframed-text', action='store_true')
    parser.add_argument('database', nargs='?')
    args = parser.parse_args()

    if args.port:
        client = SlipSerialClient(args.port, args.baudrate, args.database)
        shell = Shell(client, debug=args.debug)
        shell.cmdloop()
        client.reader.stop()
    elif args.unframed_text:
        decode_unframed_text_stream(sys.stdin, args.database)
    else:
        sys.exit('Bad input arguments.')


if __name__ == '__main__':
    main()
