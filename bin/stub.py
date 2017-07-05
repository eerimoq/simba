#!/usr/bin/env python3

import sys
import argparse
import subprocess
import re


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('crosscompile')
    parser.add_argument('objectfile')
    parser.add_argument('sourcefile')
    parser.add_argument('patterns', nargs='+')
    args = parser.parse_args()

    # Find th pattern for this source file (if any).
    found = False

    for pattern in args.patterns:
        if pattern.startswith(args.sourcefile):
            found = True
            break

    if not found:
        return

    try:
        c_source, symbols = pattern.split(':')

        if not symbols:
            return

        symbols = symbols.split(',')
    except:
        sys.exit("error: bad stub pattern '{}'".format(pattern))

    # Find all symbols in the object file.
    command = [args.crosscompile + 'readelf']
    command += ['-s']
    command += ['-W']
    command += [args.objectfile]

    readelf_output = subprocess.check_output(command)
    re_symbol = re.compile(r"\s+\d+: \w+\s+\d+\s+(NOTYPE|OBJECT|FUNC)"
                           "\s+\w+\s+\w+\s+\w+\s+(.+)")
    symbols_in_objectfile = []

    for line in readelf_output.decode('ascii').splitlines():
        mo = re_symbol.match(line)

        if mo:
            symbols_in_objectfile.append(mo.group(2).strip())

    # Redefine given symbols in the object file.
    command = [args.crosscompile + 'objcopy']

    for symbol in symbols:
        if symbol not in symbols_in_objectfile:
            sys.exit("error: cannot stub missing symbol {}".format(symbol))

        print("Stubbing symbol '{}' in '{}'.".format(symbol, c_source))

        command += [
            '--redefine-sym', '{symbol}=__stub_{symbol}'.format(symbol=symbol)
        ]

    command += [args.objectfile, args.objectfile]

    subprocess.check_call(command)


if __name__ == '__main__':
    main()
