#!/usr/bin/env python

from __future__ import print_function

import subprocess
import tarfile
import argparse
import json
import errno
import os
import shutil
import glob


ARDUINO_H = """/**
 * This is a generated file required by the Arduino build system."
 */

#include "simba.h"
"""

SETTINGS_H = """
#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#define SETTING_AREA_OFFSET     128
#define SETTING_AREA_SIZE       256
#define SETTING_AREA_CRC_OFFSET (SETTING_AREA_SIZE - 4)

#endif
"""

SIMBA_GEN_C_FMT = """
#include "simba.h"

const FAR char sysinfo[] = "app:    myapp built - by -.\\r\\n"
                           "board:  {board}\\r\\n"
                           "mcu:    {mcu}\\r\\n";
"""


def mkdir_p(path):
    """Recursivly create directories.

    """

    try:
        os.makedirs(path)
    except OSError as exc:  # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise


def extract_release_archive(archive_path):
    """Unpack given archive into current directory.

    """

    with tarfile.open(archive_path) as fin:
        fin.extractall()


def create_database():
    """Generate the simba database with baord and mcu information.

    """

    return json.loads(subprocess.check_output(["dbgen.py"]))


def generate_cores():
    """Generate the cores directory, shared among all boards.
    """

    cores_dir = os.path.join("cores", "simba")

    # Create the variant directory.
    mkdir_p(cores_dir)

    with open(os.path.join(cores_dir, "Arduino.h"), "w") as fout:
        fout.write(ARDUINO_H)

    # Copy the kernel, drivers, inet and slib packages to the cores
    # directory.
    packages = [
        "kernel",
        "drivers",
        "inet",
        "slib"
    ]

    simba_root = os.environ["SIMBA_ROOT"]

    for package in packages:
        shutil.copytree(os.path.join(simba_root, "src", package),
                        os.path.join(cores_dir, package))

    # Copy the header files.
    for header in glob.glob(os.path.join(simba_root, "src/*.h")):
        shutil.copy(header, cores_dir)

    # Remove all c source files from the drivers package. They are
    # added per variant.
    for name in os.listdir(os.path.join(cores_dir, "drivers")):
        if name in ["drivers.h", "drivers.mk", "drivers", "ports"]:
            continue

        path = os.path.join(cores_dir, "drivers", name)

        if os.path.isfile(path):
            os.remove(path)
        else:
            shutil.rmtree(path)

    # Generate a dummy settings file.
    with open(os.path.join(cores_dir, "settings.h"), "w") as fout:
        fout.write(SETTINGS_H)

    # Various files.
    root_files = [
        "LICENSE",
        "README.rst",
        "VERSION.txt"
    ]

    for root_file in root_files:
        shutil.copy(os.path.join(simba_root, root_file), ".")


def generate_variants(database):
    """Generate the variants directory with board unique information.

    """

    simba_root = os.environ["SIMBA_ROOT"]

    for board_name, config in database['boards'].items():
        avr_boards = [
            "arduino_mega",
            "arduino_uno",
            "arduino_nano"
        ]

        if board_name not in avr_boards:
            continue

        variant_dir = os.path.join("variants", "arduino", board_name)

        # Create the variant directory.
        mkdir_p(variant_dir)

        # Copy board and mcu files to the variant directory.
        boards_files = glob.glob(os.path.join(simba_root,
                                              "src",
                                              "boards",
                                              board_name,
                                              "*"))
        mcus_files = glob.glob(os.path.join(simba_root,
                                            "src",
                                            "mcus",
                                            config["mcu"],
                                            "*"))

        for path in boards_files + mcus_files:
            shutil.copy(path, variant_dir)

        # Copy the drivers c files to the variant directory.
        drivers_dir = os.path.join(variant_dir, "drivers")
        mkdir_p(drivers_dir)

        for driver in config["drivers"]:
            driver_c = os.path.join(simba_root,
                                    "src",
                                    "drivers",
                                    driver + ".c")
            shutil.copy(driver_c, drivers_dir)
 
        with open(os.path.join(variant_dir, "simba_gen.c"), "w") as fout:
            fout.write(SIMBA_GEN_C_FMT.format(name="myapp",
                                              board="board",
                                              mcu="mcu"))


def copy_configuration_files():
    """
    """

    simba_root = os.environ["SIMBA_ROOT"]

    configuration_files = [
        "boards.txt",
        "platform.txt"
    ]

    for configuration_file in configuration_files:
        shutil.copy(os.path.join(simba_root,
                                 "make",
                                 "arduino",
                                 configuration_file),
                    ".")


def generate_files_and_folders(database, outdir, remove_outdir):
    """Generate files and folders.

    """

    if remove_outdir:
        if os.path.exists(outdir):
            shutil.rmtree(outdir)

    os.makedirs(outdir)
    os.chdir(outdir)

    generate_cores()
    generate_variants(database)
    copy_configuration_files()


def main():
    """Unpack given Simba release archive and modify files and folders as
    required by the Arduino build system.

    """

    parser = argparse.ArgumentParser()
    parser.add_argument("--outdir", "-o", default="simba-arduino")
    parser.add_argument("--remove-outdir", "-r", action="store_true")
    args = parser.parse_args()

    database = create_database()
    generate_files_and_folders(database, args.outdir, args.remove_outdir)


if __name__ == "__main__":
    main()
