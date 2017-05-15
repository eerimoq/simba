#!/usr/bin/env python3
#

from __future__ import print_function

import sys
import struct
import socket
import argparse
import threading
import datetime


# Simba socket device types.
TYPE_UNSUPPORTED_TYPE                  =  0
TYPE_UART_DEVICE_REQUEST               =  1
TYPE_UART_DEVICE_RESPONSE              =  2
TYPE_PIN_DEVICE_REQUEST                =  3
TYPE_PIN_DEVICE_RESPONSE               =  4
TYPE_PWM_DEVICE_REQUEST                =  5
TYPE_PWM_DEVICE_RESPONSE               =  6
TYPE_CAN_DEVICE_REQUEST                =  7
TYPE_CAN_DEVICE_RESPONSE               =  8
TYPE_I2C_DEVICE_REQUEST                =  9
TYPE_I2C_DEVICE_RESPONSE               = 10


# Maps device type strings to request types.
REQUEST_TYPE_FROM_STRING = {
    'uart': TYPE_UART_DEVICE_REQUEST,
    'pin': TYPE_PIN_DEVICE_REQUEST,
    'pwm': TYPE_PWM_DEVICE_REQUEST,
    'can': TYPE_CAN_DEVICE_REQUEST,
    'i2c': TYPE_I2C_DEVICE_REQUEST
}


# Error codes.
ENODEV     = 19
EADDRINUSE = 98


class UnsupportedTypeError(Exception):
    pass


class NoSuchDeviceError(Exception):
    pass


class DeviceAlreadyInUseError(Exception):
    pass


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
            request += self.device_name.encode('utf-8')

            self.write(request)

            # Read the header.
            response = self.read(8)

            if len(response) != 8:
                raise RuntimeError('error: bad response length {}'.format(
                    len(response)))

            response_type, size = struct.unpack('>II', response)

            if response_type == TYPE_UNSUPPORTED_TYPE:
                raise UnsupportedTypeError()

            if response_type != request_type + 1:
                raise RuntimeError('error: bad response type {}'.format(
                    response_type))

            if size != 4:
                raise RuntimeError('error: bad response size {}'.format(
                    size))

            # Read the data.
            data = self.read(size)

            if len(data) != size:
                raise RuntimeError('error: bad response data length {}'.format(
                    len(data)))

            result = struct.unpack('>i', data)[0]

            if result != 0:
                if result == -ENODEV:
                    raise NoSuchDeviceError(
                        'error: no such device: {}'.format(self.device_name))
                elif result == -EADDRINUSE:
                    raise DeviceAlreadyInUseError(
                        'error: device already in use: {}'.format(self.device_name))
                else:
                    raise RuntimeError('error: {}: {}'.format(result,
                                                              self.device_name))

            print('done.')
        except BaseException:
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

    def readline(self):
        """Read a line.

        """

        buf = b''

        while True:
            data = self.socket.recv(1)

            if not data:
                break

            buf += data

            if data == b'\n':
                break

        return buf


def reader_main(device):
    """Reads data from the application.

    """

    while True:
        byte = device.read(1)

        if not byte:
            print('Connection closed.')
            break

        timestamp = datetime.datetime.now().strftime("%H:%M:%S.%f")
        prefix = '{} {}({}) RX:'.format(timestamp,
                                        device.device_type,
                                        device.device_name)
        print(prefix, byte)


def reader_line_main(device):
    """Reads data from the application, one line at a time.

    """

    while True:
        line = device.readline()

        if not line:
            print('Connection closed.')
            break

        line = line.strip()

        timestamp = datetime.datetime.now().strftime("%H:%M:%S.%f")
        prefix = '{} {}({}) RX:'.format(timestamp,
                                        device.device_type,
                                        device.device_name)

        try:
            print(prefix, line.decode('utf-8'))
        except UnicodeDecodeError:
            print(prefix, line)


def monitor(device_type, device_name, address, port):
    """Monitor given device.

    """

    device = SocketDevice(device_type, device_name, address, port)
    device.start()
    reader = threading.Thread(target=reader_main, args=(device, ))
    reader.setDaemon(True)
    reader.start()

    while True:
        device.write(sys.stdin.read(1).encode('utf-8'))


def monitor_line(device_type, device_name, address, port):
    """Monitor given device.

    """

    device = SocketDevice(device_type, device_name, address, port)
    device.start()
    reader = threading.Thread(target=reader_line_main, args=(device, ))
    reader.setDaemon(True)
    reader.start()

    while True:
        line = input('$ ')
        line = line.strip('\r\n')
        timestamp = datetime.datetime.now().strftime("%H:%M:%S.%f")
        prefix = '{} {}({}) TX:'.format(timestamp, device_type, device_name)
        print(prefix, line)
        line += '\r\n'
        device.write(line.encode('utf-8'))


def request_all_devices(device_type, address, port):
    """Request all devices of given type.

    """

    devices = []
    index = 0

    while True:
        try:
            device = SocketDevice(device_type, str(index), address, port)
            device.start()
            reader = threading.Thread(target=reader_main, args=(device, ))
            reader.setDaemon(True)
            reader.start()
            devices.append((device, reader))
        except DeviceAlreadyInUseError:
            pass
        except (NoSuchDeviceError, UnsupportedTypeError):
            break

        index += 1

    return devices


def request_all_line_devices(device_type, address, port):
    """Request all line devices of given type.

    """

    devices = []
    index = 0

    while True:
        try:
            device = SocketDevice(device_type, str(index), address, port)
            device.start()
            reader = threading.Thread(target=reader_line_main, args=(device, ))
            reader.setDaemon(True)
            reader.start()
            devices.append((device, reader))
        except DeviceAlreadyInUseError:
            pass
        except (NoSuchDeviceError, UnsupportedTypeError):
            break

        index += 1

    return devices


def do_pin(args):
    monitor_line('pin', args.device, args.address, args.port)


def do_uart(args):
    monitor('uart', args.device, args.address, args.port)


def do_pwm(args):
    monitor_line('pwm', args.device, args.address, args.port)


def do_can(args):
    monitor_line('can', args.device, args.address, args.port)


def do_i2c(args):
    monitor_line('i2c', args.device, args.address, args.port)


def do_monitor(args):
    uart_devices = request_all_devices('uart',
                                       args.address,
                                       args.port)
    pin_devices = request_all_line_devices('pin',
                                           args.address,
                                           args.port)
    pwm_devices = request_all_line_devices('pwm',
                                           args.address,
                                           args.port)
    can_devices = request_all_line_devices('can',
                                           args.address,
                                           args.port)
    i2c_devices = request_all_line_devices('i2c',
                                           args.address,
                                           args.port)

    input('Press <Enter> to exit.')


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('-d', '--debug', action='store_true')
    parser.add_argument('-a', '--address',
                        default='localhost',
                        help='TCP address to connect to (default: localhost).')
    parser.add_argument('-p', '--port',
                        type=int,
                        default=47000,
                        help='TCP port to connect to (default: 47000).')

    # Workaround to make the subparser required in Python 3.
    subparsers = parser.add_subparsers(title='subcommands',
                                       dest='subcommand')
    subparsers.required = True

    uart_parser = subparsers.add_parser('uart')
    uart_parser.add_argument('device', help='Uart device to request.')
    uart_parser.set_defaults(func=do_uart)

    pin_parser = subparsers.add_parser('pin')
    pin_parser.add_argument('device', help='Pin device to request.')
    pin_parser.set_defaults(func=do_pin)

    pwm_parser = subparsers.add_parser('pwm')
    pwm_parser.add_argument('device', help='Pwm device to request.')
    pwm_parser.set_defaults(func=do_pwm)

    can_parser = subparsers.add_parser('can')
    can_parser.add_argument('device', help='Can device to request.')
    can_parser.set_defaults(func=do_can)

    i2c_parser = subparsers.add_parser('i2c')
    i2c_parser.add_argument('device', help='I2c device to request.')
    i2c_parser.set_defaults(func=do_i2c)

    monitor_parser = subparsers.add_parser('monitor')
    monitor_parser.set_defaults(func=do_monitor)

    args = parser.parse_args()

    if not args.debug:
        try:
            args.func(args)
        except BaseException as e:
            sys.exit(str(e))
    else:
        args.func(args)


if __name__ == '__main__':
    main()
