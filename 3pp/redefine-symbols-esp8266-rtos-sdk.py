#!/usr/bin/env python

import subprocess
import glob
import argparse
import re
import StringIO

def find_symbols_in_libraries():
    """

    """
    
    libraries = glob.glob("ESP8266_RTOS_SDK/lib/*.a")

    command = ["xtensa-lx106-elf-readelf"]
    command += ["-s"]
    command += ["--wide"]
    command += libraries
    
    print ' '.join(command)
    output = subprocess.check_output(command)

    re_symbol = re.compile(r"\s+\w+:\s+\w+\s+\w+\s+(FUNC|OBJECT)"
                           "\s+\w+\s+\w+\s+\w+\s+(\w+)\s+")
    symbols = []
    fin = StringIO.StringIO(output)
    for line in fin.readlines():
        mo = re_symbol.match(line)
        if mo:
            symbol = mo.group(2)
            # don't redefine symbols whose name starts with "_"
            if not symbol.startswith("_"):
                symbols.append(symbol)

    redefined_symbols = []
    for symbol in symbols:
        redefined_symbol = "{symbol} esp_{symbol}".format(
            symbol=symbol)
        if redefined_symbol in redefined_symbols:
            continue
        redefined_symbols.append(redefined_symbol)

    # create the mapping file
    with open("redefine-symbols.txt", "w") as fout:
        fout.write("\n".join(redefined_symbols) + "\n")


def redefine_symbols():
    """

    """

    files = []
    files += glob.glob("ESP8266_RTOS_SDK/include/espressif/esp_sta.h")
    files += glob.glob("ESP8266_RTOS_SDK/include/*/*.h")
    files += glob.glob("ESP8266_RTOS_SDK/include/*/*/*.h")
    files += glob.glob("ESP8266_RTOS_SDK/include/*/*/*.h")
    files += glob.glob("ESP8266_RTOS_SDK/lib/*.a")

    command = ["../bin/redefine-symbols.py"]
    command += ["--toolchain-prefix", "xtensa-lx106-elf-"]
    command += ["--symbols", "redefine-symbols.txt"]
    command += files

    print ' '.join(command)
    subprocess.check_call(command) 


def main(args):
    """

    """

    if args.find_symbols_in_libraries:
        find_symbols_in_libraries()
    else:
        redefine_symbols()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument("--find-symbols-in-libraries",
                        action="store_true",
                        help=("Use readelf to find all symbols in the "
                              "libraries."))

    args = parser.parse_args()
    main(args)
