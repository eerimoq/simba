#!/usr/bin/env python3
#
# A stub of python-can.
#

import struct
from socket_device import SocketDevice


rc = {}


class Message(object):

    def __init__(self, arbitration_id, extended_id, data):
        self.arbitration_id = arbitration_id
        self.extended_id = extended_id
        self.data = data


class interface(object):
    class Bus(object):
        """A stub communicating over a TCP socket instead of a CAN bus.

        """

        def __init__(self, device, *args, **kwargs):
            del args
            del kwargs
            self.device = SocketDevice('can', device)
            self.device.start()

        def send(self, message):
            """Write given message to the application.

            """

            length = len(message.data)
            data = bytearray(message.data + b'\x00' * (8 - length))

            line = "{:08x} {} {} {:02x} {:02x} {:02x} {:02x} " \
                   "{:02x} {:02x} {:02x} {:02x}\r\n".format(
                       message.arbitration_id,
                       1 if message.extended_id else 0,
                       length,
                       data[0],
                       data[1],
                       data[2],
                       data[3],
                       data[4],
                       data[5],
                       data[6],
                       data[7])

            self.device.write(line.encode('ascii'))

        def recv(self):
            """Read a message from the application.

            """

            line = self.device.readline()
            line = line.strip(b'\r\n')

            words = line.split()
            arbitration_id = int(words[0], 16)
            extended_id = (words[1] == b'1')
            length = int(words[2])
            data = b''.join([struct.pack('B', int(word, 16))
                             for word in words[3:]])

            return Message(arbitration_id, extended_id, data[0:length])
