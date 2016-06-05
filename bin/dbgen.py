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
                                   cwd="examples/blink")


def main():
    """Main.

    """

    database = {
        "boards": dict([(board, {}) for board in get_boards()]),
        "mcus": {}
    }

    for board in database["boards"]:
        # Get the descriptive name, pinout and homepage
        for variable in ["BOARD_DESC", "BOARD_HOMEPAGE", "BOARD_PINOUT"]:
            value = get_make_variable(board, variable)
            database["boards"][board][variable.lower()] = value.strip()

        # Get the board drivers
        drivers_src = get_make_variable(board, "DRIVERS_SRC").split()
        drivers = [os.path.splitext(os.path.basename(driver))[0]
                   for driver in drivers_src]
        drivers = list(set(drivers) - set(["usb",
                                           "usb_host_class_hid",
                                           "usb_host_class_mass_storage"]))
        database["boards"][board]["drivers"] = drivers

        # get the board MCU
        mcu = get_make_variable(board, "MCU").strip()
        mcu = mcu.replace("/", "")
        database["boards"][board]["mcu"] = mcu

        if mcu not in database["mcus"]:
            database["mcus"][mcu] = {}
            # Get the descriptive name and homepage
            for variable in ["MCU_NAME", "MCU_HOMEPAGE", "ARCH"]:
                value = get_make_variable(board, variable)
                database["mcus"][mcu][variable.lower()] = value.strip()

    print json.dumps(database, indent=4)


if __name__ == "__main__":
    main()
