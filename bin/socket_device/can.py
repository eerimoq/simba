#!/usr/bin/env python3
#
# A stub of python-can.
#

import binascii
from socket_device import SocketDevice
import re


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

            line = "id={:08x},extended={},size={},data=".format(
                message.arbitration_id,
                1 if message.extended_id else 0,
                len(message.data))
            line = line.encode('ascii')
            line += binascii.hexlify(bytes(message.data))
            line += b"\r\n"

            self.device.write(line)

        def recv(self):
            """Read a message from the application.

            """

            line = self.device.readline()

            line = line.strip(b'\r\n')
            mo = re.match(rb'id=(\w{8}),extended=(\d),size=(\d),data=(\w+)',
                          line)

            arbitration_id = int(mo.group(1), 16)
            extended_id = (mo.group(2) == b'1')
            length = int(mo.group(3))
            data = bytearray(binascii.unhexlify(mo.group(4)[0:2*length]))

            return Message(arbitration_id, extended_id, data)
