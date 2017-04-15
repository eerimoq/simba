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


def connect(address, port):
    """Connect to the application.

    """

    server = socket.socket()

    try:
        print('Connecting to {}:{}... '.format(address, port),
              flush=True,
              end='')
        server.connect((address, port))
        print('done.')
    except ConnectionRefusedError:
        print('failed.', flush=True)
        raise

    return server


def request_device(server, request_type, name, device):
    """Request the device.

    """

    try:
        print('Requesting {} device {}... '.format(name, device),
              flush=True,
              end='')
        device = device.encode('ascii')
        request = struct.pack('>II', request_type, len(device))
        request += device
        server.sendall(request)
        response = server.recv(12)

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
            raise RuntimeError('error: bad response result {}'.format(
                result))

        print('done.')
    except RuntimeError:
        print('failed.', flush=True)
        raise
