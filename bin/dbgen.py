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

        simba_root = get_make_variable(board, "SIMBA_ROOT").strip()
        simba_root += '/'

        # Get board drivers.
        drivers_dir = simba_root + "src/drivers/"
        drivers_src = get_make_variable(board, "DRIVERS_SRC").split()
        drivers_src = [s.replace(drivers_dir, "") for s in drivers_src]
        drivers = []
        for src in drivers_src:
            if not os.path.split(src)[0]:
                drivers.append(os.path.splitext(src)[0])
        database["boards"][board]["drivers"] = drivers


        # Get board include paths.
        inc = get_make_variable(board, "INC").split()
        inc = [i.replace(simba_root, "") for i in inc]
        inc = list(set(inc) - set("."))
        database["boards"][board]["inc"] = inc

        # Get board sources.
        src = get_make_variable(board, "SRC").split()
        src = list(set(src) - set(["main.c", "settings.c"]))
        src = [s.replace(simba_root, "") for s in src]
        database["boards"][board]["src"] = src

        # Get CFLAGS.
        cflags = get_make_variable(board, "CFLAGS").split()
        database["boards"][board]["cflags"] = cflags

        # Get CDEFS.
        cdefs = get_make_variable(board, "CDEFS").split()
        database["boards"][board]["cdefs"] = cdefs

        # Get LDFLAGS.
        ldflags = get_make_variable(board, "LDFLAGS").split()
        database["boards"][board]["ldflags"] = ldflags

        # Get LIBPATH.
        libpath = get_make_variable(board, "LIBPATH").split()
        libpath = [l.replace(simba_root, "") for l in libpath]
        database["boards"][board]["libpath"] = libpath

        # Get linker file.
        try:
            linker_script = get_make_variable(board,
                                              "LINKER_SCRIPT").strip()
            linker_script = linker_script.replace(simba_root, "")
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
