#!/usr/bin/env python
#

from __future__ import print_function

import sys
import argparse
import re
import base64


def canonical(filename):
    canonical_filename = ''

    for char in filename:
        if re.match(r'[a-zA-Z0-9_]', char):
            canonical_filename += char
        else:
            canonical_filename += '_'

    return canonical_filename


def pack_c_string(c_string):
    string = ''

    in_c_string = False
    prev_char = None

    for char in c_string:
        if in_c_string:
            if prev_char == '\\':
                if char == '\\':
                    prev_char = None
                else:
                    prev_char = char
                string += char
            else:
                if prev_char != '\\' and char == '"':
                    in_c_string = False
                else:
                    string += char
                prev_char = char
        elif char == '"':
            in_c_string = True

    return '"' + string + '"'


def process_format_strings(contents, canonical_filename):
    """Find all format strings defined with FMTSTR().

    """

    format_strings = []
    output_contents = ''
    begin = 0
    end = 0

    while True:
        begin = contents.find('__simbapp_fmtstr_begin__', begin)

        if begin == -1:
            output_contents += contents[end:]
            break

        output_contents += contents[end:begin]

        end = contents.find('__simbapp_fmtstr_end__', begin)

        if end == -1:
            print('End of log point missing.')
            sys.exit(1)

        format_string = pack_c_string(contents[begin+25:end-1])
        formats = re.findall(r'%-?[0-9]*?l?[dsuxcf]', format_string)
        soam_format_string = '"' + '\\x1f'.join(formats) + '"'
        decoder_format_string = re.sub(r'%-?[0-9]*?l?[dsuxcf]', "{}",
                                       format_string)
        decoder_format_string = re.sub(r'%%', "%", decoder_format_string)
        c_variable = '__fmt_' + base64.b32encode(format_string).replace('=', '_').lower()
        format_strings.append((format_string,
                               c_variable,
                               soam_format_string,
                               decoder_format_string))
        output_contents += c_variable

        end += 22
        begin = end

    declarations = ''

    for _, c_variable, _, _ in format_strings:
        declarations += 'extern const FAR char {}[];\n'.format(c_variable)

    return declarations + output_contents, format_strings


def process_commands(contents, canonical_filename):
    """Find all format strings defined with CMDSTR().

    """

    commands = []
    output_contents = ''
    begin = 0
    end = 0

    while True:
        begin = contents.find('__simbapp_cmdstr_begin__', begin)

        if begin == -1:
            output_contents += contents[end:]
            break

        output_contents += contents[end:begin]

        end = contents.find('__simbapp_cmdstr_end__', begin)

        if end == -1:
            print('End of command missing.')
            sys.exit(1)

        command = pack_c_string(contents[begin+25:end-1])
        c_variable = '__cmd_' + base64.b32encode(command).replace('=', '_').lower()
        commands.append((command, c_variable))
        output_contents += c_variable

        end += 22
        begin = end

    declarations = ''

    for _, c_variable in commands:
        declarations += 'extern const FAR char {}[];\n'.format(c_variable)

    return declarations + output_contents, commands


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-o', '--output', required=True)
    parser.add_argument('filename')
    args = parser.parse_args()

    with open(args.filename, 'rb') as fin:
        contents = fin.read()

    contents, format_strings = process_format_strings(contents,
                                                      canonical(args.filename))
    contents, commands = process_commands(contents, canonical(args.filename))
    contents = '#include "far.h"\n' + contents

    with open(args.output, 'wb') as fout:
        fout.write(contents)

    with open(args.output + '.db', 'wb') as fout:
        for format_string in format_strings:
            fout.write('FMT\n{}\n{}\n{}\n{}\n\n'.format(*format_string))

        for command in commands:
            fout.write('CMD\n{}\n{}\n\n'.format(*command))


if __name__ == '__main__':
    main()
