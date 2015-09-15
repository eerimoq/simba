#!/usr/bin/env python

import sys
from ConfigParser import ConfigParser
from collections import OrderedDict


SETTINGS_FILE = "simba_settings.bin"


def parse_settings_file(filename):
    settings_parser = ConfigParser()
    settings_parser.read(filename)

    addresses = []
    sizes = []
    values = []
    types = []

    for item in settings_parser.items("addresses"):
        addresses.append((item[0], int(item[1], 0)))

    for item in settings_parser.items("sizes"):
        sizes.append((item[0], int(item[1], 0)))

    for item in settings_parser.items("types"):
        types.append((item[0], item[1]))

    for item in settings_parser.items("values"):
        values.append((item[0], item[1]))

    addresses.sort(key=lambda item: item[1])

    return addresses, sizes, types, values


def create_settings_dict(addresses, sizes, types, values):
    settings = OrderedDict()

    for name, address in addresses:
        settings[name] = {"address": address}

    for name, size in sizes:
        if name not in settings:
            sys.stderr.write("{}: no address for settings\n".format(name))
            sys.exit(1)
        settings[name]["size"] = size

    for name, type in types:
        if name not in settings:
            sys.stderr.write("{}: no address for settings\n".format(name))
            sys.exit(1)
        settings[name]["type"] = type

    for name, value in values:
        if name not in settings:
            sys.stderr.write("{}: no address for settings\n".format(name))
            sys.exit(1)
        settings[name]["value"] = value

    return settings

if __name__ == "__main__":
    items = parse_settings_file(sys.argv[1])
    settings = create_settings_dict(*items)

    # create the settings file content
    content = ''
    for name, item in settings.items():
        # add padding between previous setting and this one
        content += '\xff' * (item["address"] - len(content))
        # add the value
        if item["type"] == "string":
            if item["size"] <= len(item["value"]):
                sys.stderr.write("{}: value does not fit in size {}\n".format(item["value"],
                                                                              item["size"]))
                sys.exit(1)
            content += item["value"]
            # null termination
            content += "\x00"
        else:
            sys.stderr.write("{}: bad type\n".format(item["type"]))
            sys.exit(1)

    # write the content to the settings file
    with open(SETTINGS_FILE, "wb") as fout:
        fout.write(content)
