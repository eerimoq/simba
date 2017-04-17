#!/usr/bin/env python3
#
# A stub of python-can.
#

import binascii
from socket_device import SocketDevice


rc = {}


class Message(object):

    def __init__(self, arbitration_id, extended_id, data):
        self.arbitration_id = arbitration_id
        self.extended_id = extended_id
        self.data = bytearray(data)

    def __repr__(self):
        return 'Message(arbitration_id={}, extended_id={}, data={})'.format(
            self.arbitration_id,
            self.extended_id,
            self.data)


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
            data = message.data + b'\x00' * (8 - length)

            line = "{:08x},{},{},".format(message.arbitration_id,
                                          1 if message.extended_id else 0,
                                          length)
            line = line.encode('ascii')
            line += binascii.hexlify(bytes(data))
            line += b"\r\n"

            self.device.write(line)

        def recv(self):
            """Read a message from the application.

            """

            line = self.device.readline()
            line = line.strip(b'\r\n')

            words = line.split(b',')
            arbitration_id = int(words[0], 16)
            extended_id = (words[1] == b'1')
            length = int(words[2])
            data = bytearray(binascii.unhexlify(words[3][0:2*length]))

            return Message(arbitration_id, extended_id, data)
