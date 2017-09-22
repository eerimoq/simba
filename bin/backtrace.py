#!/usr/bin/env python3

import sys
import re
import subprocess


def main():
    program_name = sys.argv[1]
    cross_compile = sys.argv[2]
    re_backtrace_address = re.compile(r'^(  \[\d+\]: )(0x[0-9a-f]+)')

    for line in sys.stdin.readlines():
        mo = re_backtrace_address.match(line)

        if mo:
            line = mo.group(1)
            address = mo.group(2)
            command = [
                cross_compile + 'addr2line',
                '-f',
                '-p',
                '-e', program_name,
                address
            ]
            line += subprocess.check_output(command).decode('utf-8')

        print(line, end='')


if __name__ == '__main__':
    main()
