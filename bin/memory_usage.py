#!/usr/bin/env python

import sys
import argparse
from elftools.elf.elffile import ELFFile


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--ram-section", action="append", required=True)
    parser.add_argument("--rom-section", action="append", required=True)
    parser.add_argument("elffile")
    args = parser.parse_args()

    with open(args.elffile, 'rb') as f:
        elffile = ELFFile(f)
        ram_usage = 0
        rom_usage = 0

        # RAM usage
        for name in args.ram_section:
            section = elffile.get_section_by_name(name)
            if section is None:
                print("Bad section name " + name)
                sys.exit(1)
            ram_usage += section["sh_size"]

        # ROM usage
        for name in args.rom_section:
            section = elffile.get_section_by_name(name)
            if section is None:
                print("Bad section name " + name)
                sys.exit(1)
            rom_usage += section["sh_size"]

    print('{{\"program\": {}, \"data\": {}}}'.format(rom_usage, ram_usage))


if __name__ == "__main__":
    main()
