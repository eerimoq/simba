#!/usr/bin/env python3

import sys
import argparse
import subprocess


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('crosscompile')
    parser.add_argument('objectfile')
    parser.add_argument('pattern', nargs='?')
    args = parser.parse_args()

    if not args.pattern:
        return

    try:
        symbols = args.pattern.split(':')[1]

        if not symbols:
            return

        symbols = symbols.split(',')
    except:
        sys.exit("error: bad stub pattern '{}'".format(args.pattern))

    command = [args.crosscompile + 'objcopy']

    for symbol in symbols:
        print('stubbing symbol', symbol)
        command += [
            '--redefine-sym', '{symbol}=__stub_{symbol}'.format(symbol=symbol)
        ]

    command += [args.objectfile, args.objectfile]

    subprocess.check_call(command)


if __name__ == '__main__':
    main()
