#!/usr/bin/env python3
#

from __future__ import print_function

import struct
import socket


# Simba socket device types.
TYPE_UART_DEVICE_REQUEST               = 1
TYPE_UART_DEVICE_RESPONSE              = 2
TYPE_PIN_DEVICE_REQUEST                = 3
TYPE_PIN_DEVICE_RESPONSE               = 4
TYPE_PWM_DEVICE_REQUEST                = 5
TYPE_PWM_DEVICE_RESPONSE               = 6


# Maps device type strings to request types.
REQUEST_TYPE_FROM_STRING = {
    'uart': TYPE_UART_DEVICE_REQUEST,
    'pin': TYPE_PIN_DEVICE_REQUEST,
    'pwm': TYPE_PWM_DEVICE_REQUEST
}


# Error codes.
ENODEV     = 19
EADDRINUSE = 98


class SocketDevice(object):

    def __init__(self, device_type, device_name, address=None, port=None):
        self.device_type = device_type
        self.device_name = device_name

        if address is None:
            address = 'localhost'

        self.address = address

        if port is None:
            port = 47000

        self.port = port
        self.socket = socket.socket()

    def start(self):
        """Connect to the application and request the device.

        """

        self.connect()
        self.request_device()

    def stop(self):
        """Close the connection to the application.

        """

        self.socket.close()

    def connect(self):
        """Connect to the application.

        """

        try:
            print('Connecting to {}:{}... '.format(self.address, self.port),
                  flush=True,
                  end='')
            self.socket.connect((self.address, self.port))
            print('done.')
        except ConnectionRefusedError:
            print('failed.', flush=True)
            raise

    def request_device(self):
        """Request the device.

        """

        try:
            print('Requesting {} device {}... '.format(self.device_type,
                                                       self.device_name),
                  flush=True,
                  end='')

            request_type = REQUEST_TYPE_FROM_STRING[self.device_type]
            request = struct.pack('>II', request_type, len(self.device_name))
            request += self.device_name.encode('ascii')

            self.write(request)
            response = self.read(12)

            if len(response) != 12:
                raise RuntimeError('error: bad response length {}'.format(
                    len(response)))

            response_type, size, result = struct.unpack('>IIi', response)

            if response_type != request_type + 1:
                raise RuntimeError('error: bad response type {}'.format(
                    response_type))

            if size != 4:
                raise RuntimeError('error: bad response size {}'.format(size))

            if result != 0:
                if result == -ENODEV:
                    message = 'no such device'
                elif result == -EADDRINUSE:
                    message = 'device already in use'
                else:
                    message = '{}'.format(result)

                raise RuntimeError('error: {}: {}'.format(message,
                                                          self.device_name))

            print('done.')
        except RuntimeError:
            print('failed.', flush=True)
            raise

    def write(self, buf):
        self.socket.sendall(buf)

    def read(self, length=1):
        buf = b''

        while len(buf) < length:
            data = self.socket.recv(length - len(buf))

            if not data:
                break

            buf += data

        return buf
