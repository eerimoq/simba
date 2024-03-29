#!/usr/bin/env python3

import sys
import time
import getpass
import hashlib
import lzma
import argparse
import struct
import os
import binascii
import re
import zlib

try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser

from collections import OrderedDict


MAJOR = 1
MINOR = 2


SIMBA_GEN_H_FMT = """\
/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

/**
 * This file was generated by simbagen.py {major}.{minor} {date}.
 */

#ifndef __SIMBA_GEN_H__
#define __SIMBA_GEN_H__

#include "simba.h"

{settings}

#endif
"""


SIMBA_GEN_C_FMT = """\
/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

/**
 * This file was generated by simbagen.py {major}.{minor} {date}.
 */

#include "simba.h"

{sysinfo}

{settings}

{eeprom_soft}

{eeprom}

{soamdb}
"""

SYSINFO_FMT = """\
const FAR char sysinfo[] = "app:    {name}-{version} built {date} by {user}.\\r\\n"
                           "board:  {board}\\r\\n"
                           "mcu:    {mcu}\\r\\n";
"""


SETTINGS_FMT = """
{names}

{functions}

const FAR struct setting_t settings[] = {{
{array}
    {{ NULL, 0, 0, 0 }}
}};

const FAR uint8_t settings_default[CONFIG_SETTINGS_AREA_SIZE] = {{{default_data}}};
"""

SETTING_HELPER_FUNCTIONS = """\
static int setting_read(void *dst_p, size_t src, size_t size)
{
    ssize_t res;

    res = settings_read(dst_p, src, size);

    return (res == size ? 0 : res);
}

static int setting_write(size_t dst, const void *src_p, size_t size)
{
    ssize_t res;

    res = settings_write(dst, src_p, size);

    return (res == size ? 0 : res);
}
"""

SETTING_INT32_H_FMT = """\
int setting_{name}_read(int32_t *value_p);
int setting_{name}_write(int32_t value);
"""


SETTING_STRING_H_FMT = """\
int setting_{name}_read(char *value_p);
int setting_{name}_write(const char *value_p);
"""


SETTING_BLOB_H_FMT = """\
int setting_{name}_read(void *value_p);
int setting_{name}_write(const void *value_p);
"""


SETTING_INT32_C_FMT = """\
int setting_{name}_read(int32_t *value_p)
{{
    return (setting_read(value_p,
                         SETTING_{name_upper}_ADDR,
                         SETTING_{name_upper}_SIZE));
}}

int setting_{name}_write(int32_t value)
{{
    return (setting_write(SETTING_{name_upper}_ADDR,
                          &value,
                          SETTING_{name_upper}_SIZE));
}}
"""


SETTING_STRING_C_FMT = """\
int setting_{name}_read(char *value_p)
{{
    return (setting_read(value_p,
                         SETTING_{name_upper}_ADDR,
                         SETTING_{name_upper}_SIZE));
}}

int setting_{name}_write(const char *value_p)
{{
    return (setting_write(SETTING_{name_upper}_ADDR,
                          value_p,
                          SETTING_{name_upper}_SIZE));
}}
"""


SETTING_BLOB_C_FMT = """\
int setting_{name}_read(void *value_p)
{{
    return (setting_read(value_p,
                         SETTING_{name_upper}_ADDR,
                         SETTING_{name_upper}_SIZE));
}}

int setting_{name}_write(const void *value_p)
{{
    return (setting_write(SETTING_{name_upper}_ADDR,
                          value_p,
                          SETTING_{name_upper}_SIZE));
}}
"""

EEPROM_SOFT_FMT = """
uint8_t nvm_eeprom_soft_block_0[CONFIG_NVM_EEPROM_SOFT_BLOCK_0_SIZE]
__attribute__ ((section (".nvm.eeprom_soft.block_0"))) = {{{data}}};
"""


EEPROM_FMT = """
uint8_t nvm_eeprom_area[CONFIG_NVM_SIZE]
__attribute__ ((section (".eeprom"))) = {{{data}}};
"""


def unescape(value):
    return value.encode().decode('unicode-escape')


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
                value = unescape(item["value"])[1:-1]

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

        endianess_prefix = ">" if self.endianess == "big" else "<"

        for name, item in self.settings.items():
            print('Adding setting {} at address 0x{:x}.'.format(
                name,
                item["address"]))

            if item["type"] == "string_t":
                data = unescape(item["value"])[1:-1].encode() + b'\x00'
            elif item["type"] == "blob_t":
                data = binascii.unhexlify(item["value"])
            elif item["type"] == 'int32_t':
                data = struct.pack(endianess_prefix + 'i', int(item["value"], 0))
            else:
                sys.exit("error: {}: bad setting type".format(item["type"]))

            if item["address"] < len(self.binary):
                sys.exit("error: {}: address overlapping".format(
                    item["address"]))

            # Padding from last setting end to this setting start.
            if item["address"] > len(self.binary):
                self.binary += (item["address"] - len(self.binary))* b'\xff'

            self.binary += data

    def as_binary(self):
        """Get the binary from given settings.

        """

        return self.binary

    def as_simba_gen_h_section(self):
        """Create the header file.

        """

        addresses = []
        sizes = []
        types = []
        values = []
        functions = []

        for name, item in self.settings.items():
            addresses.append(
                "#define SETTING_{name}_ADDR {value}".format(
                    name=name.upper(),
                    value=item["address"]))
            sizes.append(
                "#define SETTING_{name}_SIZE {value}".format(
                    name=name.upper(),
                    value=item["size"]))
            types.append(
                "#define SETTING_{name}_TYPE setting_type_{value}".format(
                    name=name.upper(),
                    value=item["type"]))
            values.append(
                "#define SETTING_{name}_VALUE {value}".format(
                    name=name.upper(),
                    value=item["value"]))

            if item["type"] == 'int32_t':
                fmt = SETTING_INT32_H_FMT
            elif item["type"] == 'string_t':
                fmt = SETTING_STRING_H_FMT
            else:
                fmt = SETTING_BLOB_H_FMT

            functions.append(fmt.format(name=name,
                                        name_upper=name.upper()))

        return ("\n".join(addresses)
                + "\n\n"
                + "\n".join(sizes)
                + "\n\n"
                + "\n".join(types)
                + "\n\n"
                + "\n".join(values)
                + "\n\n"
                + "\n".join(functions))

    def as_simba_gen_c_section(self):
        names = []
        functions = []
        array = []

        for name, item in self.settings.items():
            names.append(
                "static const FAR char {name}_name[] = \"{name}\";".format(
                    name=name))

            if item["type"] == 'int32_t':
                fmt = SETTING_INT32_C_FMT
            elif item["type"] == 'string_t':
                fmt = SETTING_STRING_C_FMT
            else:
                fmt = SETTING_BLOB_C_FMT

            functions.append(fmt.format(name=name,
                                        name_upper=name.upper()))

            array.append(
                "    {{ .name_p = {name}_name, .type = setting_type_{type}, "
                ".address = {address}, "
                ".size = {size} }},".format(name=name,
                                            type=item["type"],
                                            address=item["address"],
                                            size=item["size"]))

        if len(functions) > 0:
            functions.insert(0, SETTING_HELPER_FUNCTIONS)

        default_data = ', '.join([str(byte)
                                  for byte in bytearray(self.as_binary())])

        return SETTINGS_FMT.format(names='\n'.join(names),
                                   functions='\n'.join(functions),
                                   array='\n'.join(array),
                                   default_data=default_data)


class EepromSoft(object):

    def __init__(self, binary, chunk_size, endianess):
        binary += b'\xff' * (chunk_size - 8 - len(binary))
        crc = zlib.crc32(binary) & 0xffffffff
        endianess_prefix = ">" if endianess == "big" else "<"
        header = struct.pack(endianess_prefix + "I", crc)
        header += struct.pack(endianess_prefix + "H", 1)
        header += struct.pack(endianess_prefix + "H", 0xa5c3)

        self.binary = header + binary

    def as_binary(self):
        return self.binary

    def as_simba_gen_c_section(self):
        data = ', '.join([str(byte) for byte in bytearray(self.binary)])

        return EEPROM_SOFT_FMT.format(data=' ' + data + ' ')


class Eeprom(object):

    def __init__(self, binary):
        self.binary = binary

    def as_binary(self):
        return self.binary

    def as_simba_gen_c_section(self):
        data = ', '.join([str(byte) for byte in bytearray(self.binary)])

        return EEPROM_FMT.format(data=' ' + data + ' ')


class SoamDb(object):

    def __init__(self, dbfiles):
        self.id = 0x8000
        self.formats = []
        self.format_string_definitions = ''
        self.decoder_format_strings = ''
        self.commands = []
        self.command_definitions = ''
        self.decoder_commands = ''

        for dbfile in dbfiles:
            with open(dbfile) as fin:
                while True:
                    line = fin.readline()

                    if not line:
                        break

                    if line.startswith('FMT'):
                        self.parse_fmt(fin)
                    elif line.startswith('CMD'):
                        self.parse_cmd(fin)
                    else:
                        sys.exit('error: {}: bad line'.format(line))

    def write_to_file(self, filename):
        date = time.strftime("%Y-%m-%d %H:%M %Z")

        with open(filename, 'w') as fout:
            fout.write('# This file was generated by simbagen.py '
                       '{major}.{minor} {date}.\n'.format(
                           major=MAJOR,
                           minor=MINOR,
                           date=date))
            fout.write(self.decoder_format_strings)
            fout.write(self.decoder_commands)

    def as_simba_gen_c_section(self, filename):
        with open(filename, 'rb') as fin:
            database_id = hashlib.md5(fin.read()).hexdigest()
            database_id = 'const char soam_database_id[] = "{}";\n\n'.format(
                database_id)

        # Compress the database.
        with open(filename, 'rb') as fin:
            if 'lzma' in sys.modules or 'backports.lzma' in sys.modules:
                compressed = b'\x00' + lzma.compress(fin.read())
            else:
                compressed = b'\x01' + fin.read()

        fmt = 'const size_t soam_database_compressed_size = {};\n\n'
        compressed_database_size = fmt.format(len(compressed))
        data = ', '.join([str(byte) for byte in bytearray(compressed)])
        fmt = 'const FAR uint8_t soam_database_compressed[] = {{ {} }};\n\n'
        compressed_database = fmt.format(data)

        return (database_id
                + compressed_database_size
                + compressed_database
                + self.format_string_definitions
                + self.command_definitions)

    def parse_fmt(self, fin):
        original_fmtstr = fin.readline().strip()
        c_variable = fin.readline().strip()
        soam_fmtstr = fin.readline().strip()
        decoder_fmtstr = fin.readline().strip()
        fin.readline()

        if original_fmtstr in self.formats:
            return

        self.formats.append(original_fmtstr)
        generated_id = self.generate_id()
        soam_fmtstr = '"\\x{:02x}\\x{:02x}"'.format(
            (generated_id >> 8) & 0xff,
            generated_id & 0xff) + soam_fmtstr
        self.format_string_definitions += 'const FAR char {}[] = {};\n'.format(
            c_variable,
            soam_fmtstr)
        self.decoder_format_strings += 'FMT: 0x{:x} {}\n'.format(
            generated_id,
            decoder_fmtstr)

    def parse_cmd(self, fin):
        command = fin.readline().strip()
        c_variable = fin.readline().strip()
        fin.readline()

        if command in self.commands:
            return

        self.commands.append(command)
        generated_id = self.generate_id()
        soam_command = '"/\\x{:02x}\\x{:02x}"'.format(
            (generated_id >> 8) & 0xff,
            generated_id & 0xff)
        self.command_definitions += 'const FAR char {}[] = {};\n'.format(
            c_variable,
            soam_command)
        self.decoder_commands += 'CMD: 0x{:x} {}\n'.format(
            generated_id,
            command)

    def generate_id(self):
        """Generate a unique identifier. It may not contain the c-string
        termination character \x00.

        """

        self.id += 1

        # The command parser function does not accept string
        # termination (0x00), quotes (0x22) or percent sign (0x25) in
        # the command string. Also, whitespace characters are stripped
        # from the command string.
        ignore_chars = [
            0x00, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x20, 0x22, 0x25
        ]

        while (self.id & 0xff) in ignore_chars:
            self.id += 1

        if self.id == 0xffff:
            sys.exit("error: too many soamdb ids")

        return self.id


def do_header(args):
    """Generate the header file.

    """

    date = time.strftime("%Y-%m-%d %H:%M %Z")

    settings = Settings(args.settings, args.endianess)
    settings_section = settings.as_simba_gen_h_section()

    os.chdir(args.output_directory)

    with open('simba_gen.h', 'w') as fout:
        fout.write(SIMBA_GEN_H_FMT.format(
            major=MAJOR,
            minor=MINOR,
            date=date,
            settings=settings_section))


def do_source(args):
    """Generate the source file.

    """

    date = time.strftime("%Y-%m-%d %H:%M %Z")

    sysinfo_seciton = SYSINFO_FMT.format(name=args.name,
                                         version=args.version,
                                         date=date,
                                         user=getpass.getuser(),
                                         board=args.board,
                                         mcu=args.mcu)

    settings = Settings(args.settings, args.endianess)
    settings_section = settings.as_simba_gen_c_section()
    settings_binary = settings.as_binary()

    eeprom_soft = EepromSoft(settings_binary,
                             args.eeprom_soft_chunk_size,
                             args.endianess)
    eeprom_soft_section = eeprom_soft.as_simba_gen_c_section()

    eeprom = Eeprom(settings_binary)
    eeprom_section = eeprom.as_simba_gen_c_section()

    soamdb = SoamDb(args.dbfiles)

    os.chdir(args.output_directory)
    soamdb.write_to_file(args.name + '.soamdb')
    soamdb_section = soamdb.as_simba_gen_c_section(args.name + '.soamdb')

    with open('simba_gen.c', 'w') as fout:
        fout.write(SIMBA_GEN_C_FMT.format(
            major=MAJOR,
            minor=MINOR,
            date=date,
            sysinfo=sysinfo_seciton,
            settings=settings_section,
            eeprom_soft=eeprom_soft_section,
            eeprom=eeprom_section,
            soamdb=soamdb_section))

    with open('eeprom_soft.bin', 'wb') as fout:
        fout.write(eeprom_soft.as_binary())

    with open('eeprom.bin', 'wb') as fout:
        fout.write(eeprom.as_binary())


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('-o', '--output-directory', default='.')

    subparsers = parser.add_subparsers()

    header_parser = subparsers.add_parser('header')
    header_parser.add_argument('-e', '--endianess', required=True)
    header_parser.add_argument('--settings')
    header_parser.set_defaults(func=do_header)

    source_parser = subparsers.add_parser('source')
    source_parser.add_argument('-n', '--name', required=True)
    source_parser.add_argument('-v', '--version', required=True)
    source_parser.add_argument('-b', '--board', required=True)
    source_parser.add_argument('-m', '--mcu', required=True)
    source_parser.add_argument('-e', '--endianess', required=True)
    source_parser.add_argument('-c', '--eeprom-soft-chunk-size',
                               type=int,
                               required=True)
    source_parser.add_argument('--settings')
    source_parser.add_argument('dbfiles', nargs="*")
    source_parser.set_defaults(func=do_source)

    args = parser.parse_args()

    args.func(args)


if __name__ == '__main__':
    main()
