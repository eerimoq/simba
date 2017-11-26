#!/usr/bin/env python

import sys
import argparse
import re
import struct
import binascii

try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser

from collections import OrderedDict


class Settings(object):

    def __init__(self, filename, endianess):
        """Parse given settings file and return its contents as a ordered
        dictionary, sorted by address.

        """

        self.binary = b''
        self.endianess = endianess
        self.settings = OrderedDict()

        if filename is None:
            return

        settings_parser = ConfigParser()
        if not settings_parser.read(filename):
            sys.exit("Failed to load config file '{}'.".format(filename))

        addresses = []
        sizes = {}
        values = {}
        types = {}

        for item in settings_parser.items("addresses"):
            addresses.append((item[0], int(item[1], 0)))

        for item in settings_parser.items("sizes"):
            sizes[item[0]] =  int(item[1], 0)

        for item in settings_parser.items("types"):
            types[item[0]] = item[1]

        for item in settings_parser.items("values"):
            values[item[0]] =  item[1]

        addresses.sort(key=lambda item: item[1])

        for name, address in addresses:
            self.settings[name] = {"address": address}

        for name in self.settings.keys():
            if len(name) > 40:
                sys.exit("error: {}: setting name over 40 characters".format(name))

            if name not in sizes:
                sys.exit("error: {}: no size found for setting".format(name))

            self.settings[name]["size"] = sizes[name]

            if name not in types:
                sys.exit("error: {}: no type found for setting".format(name))

            self.settings[name]["type"] = types[name]

            if name not in values:
                sys.exit("error: {}: no value found for setting".format(name))

            self.settings[name]["value"] = values[name]

        # Add string quotes if missing.
        for name, item in self.settings.items():
            if item["type"] == "string_t":
                value = item["value"]

                if len(value) == 0:
                    value = '""'

                if not value.startswith('"'):
                    value = '"' + value

                if not value.endswith('"'):
                    value += '"'

                item["value"] = value

        # Validate the settings.
        address = 0

        for name, item in self.settings.items():
            if item["type"] == "int32_t":
                if item["size"] != 4:
                    sys.exit("error: {}: bad int32_t size".format(item["size"]))
            elif item["type"] == "string_t":
                value = item["value"].decode('string_escape')[1:-1]

                if len(value) >= item["size"]:
                    sys.exit("error: {}: string value too long".format(
                        item["value"]))
            elif item["type"] == "blob_t":
                mo = re.match(r"^([0-9a-fA-F]{2})*$", item["value"])

                if not mo:
                    sys.exit("error: {}: bad blob value".format(item["value"]))

                if len(item["value"]) // 2 > item["size"]:
                    sys.exit("error: {}: blob value too long".format(
                        item["value"]))

            if item["address"] < address:
                sys.exit("error: {}: overlapping address".format(item["address"]))

            address = item["address"] + item["size"]


def do_decode(args):
    settings = Settings(args.settings, args.endianess)

    with open(args.binfile, 'rb') as fin:
        print('[values]')

        for name, item in settings.settings.items():
            fin.seek(item['address'])
            data = fin.read(item['size'])

            if item['type'] == 'int32_t':
                if args.endianess == 'big':
                    value = struct.unpack('>I', data)[0]
                else:
                    value = struct.unpack('<I', data)[0]
            elif item['type'] == 'string_t':
                value = data.split(b'\x00')[0].encode('string_escape')
            elif item['type'] == 'blob_t':
                value = binascii.hexlify(data)

            print('{} = {}'.format(name, value))


def main():
    parser = argparse.ArgumentParser()

    subparsers = parser.add_subparsers()

    decode_parser = subparsers.add_parser('decode')
    decode_parser.add_argument('--settings')
    decode_parser.add_argument('--endianess')
    decode_parser.add_argument('binfile')
    decode_parser.set_defaults(func=do_decode)

    args = parser.parse_args()

    args.func(args)


if __name__ == '__main__':
    main()
