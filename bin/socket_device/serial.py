#!/usr/bin/env python3
#
# A stub of pyserial.
#

from socket_device import SocketDevice


class Serial(object):
    """A stub communicating over a TCP socket instead of a serial port.

    """

    def __init__(self, device, *args, **kwargs):
        del args
        del kwargs

        self.device = SocketDevice('uart', device)
        self.device.start()

    def close(self):
        self.device.stop()

    def write(self, buf):
        self.device.write(buf)

    def read(self, length=1):
        return self.device.read(length)
