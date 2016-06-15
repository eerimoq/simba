#!/usr/bin/env python
#

import os
import subprocess
import glob
import json


def get_boards():
    """Get a list of all boards.

    """

    paths = glob.glob("src/boards/*")

    return [os.path.basename(path) for path in paths]


def get_mcus():
    """Get a list of all mcus.

    """

    paths = glob.glob("src/mcus/*")

    return [os.path.basename(path) for path in paths]


def get_make_variable(board, variable):
    """Get the value of given make variable.
    """

    return subprocess.check_output(["make",
                                    "-s",
                                    "BOARD=" + board,
                                    "print-" + variable],
                                   cwd="tst/kernel/sys")


def main():
    """Main.

    """

    database = {
        "boards": dict([(board, {}) for board in get_boards()]),
        "mcus": {}
    }

    for board in database["boards"]:
        # Get the descriptive name, pinout and homepage
        variables = [
            "BOARD_DESC",
            "BOARD_HOMEPAGE",
            "BOARD_PINOUT"
        ]
        for variable in variables:
            value = get_make_variable(board, variable)
            database["boards"][board][variable.lower()] = value.strip()

        # Get the board include.
        inc = get_make_variable(board, "INC").split()
        inc = [i.replace("../../../", "") for i in inc]
        inc = list(set(inc) - set("."))
        database["boards"][board]["inc"] = inc

        # Get the board sources.
        src = get_make_variable(board, "SRC").split()
        src = list(set(src) - set(["main.c", "settings.c"]))
        src = [s.replace("../../../", "") for s in src]
        src = [s.replace("src/inet/../../", "") for s in src]
        database["boards"][board]["src"] = src

        # Get the CFLAGS.
        cflags = get_make_variable(board, "CFLAGS").split()
        database["boards"][board]["cflags"] = cflags

        # Get the CDEFS.
        cdefs = get_make_variable(board, "CDEFS").split()
        database["boards"][board]["cdefs"] = cdefs

        # Get the LDFLAGS.
        ldflags = get_make_variable(board, "LDFLAGS").split()
        database["boards"][board]["ldflags"] = ldflags

        # Get the LIBPATH.
        libpath = get_make_variable(board, "LIBPATH").split()
        libpath = [l.replace("../../../", "") for l in libpath]
        database["boards"][board]["libpath"] = libpath

        # get linker file
        try:
            linker_script = get_make_variable(board,
                                              "LINKER_SCRIPT").strip()
            linker_script = linker_script.replace("../../../", "")
        except:
            linker_script = ""

        database["boards"][board]["linker_script"] = linker_script
        
        # get the board MCU
        mcu = get_make_variable(board, "MCU").strip()
        mcu = mcu.replace("/", "")
        database["boards"][board]["mcu"] = mcu

        if mcu not in database["mcus"]:
            database["mcus"][mcu] = {}
            # Get the descriptive name and homepage
            for variable in ["MCU_NAME", "MCU_HOMEPAGE", "ARCH", "FAMILY"]:
                value = get_make_variable(board, variable)
                database["mcus"][mcu][variable.lower()] = value.strip()

    print json.dumps(database, indent=4)


if __name__ == "__main__":
    main()
