#!/usr/bin/env python

from __future__ import print_function

import subprocess
import argparse
import json
import os
import re


SIMBA_GEN_C_FMT = """
#include "simba.h"

const FAR char sysinfo[] = "app:    myapp built - by -.\\r\\n"
                           "board:  {board}\\r\\n"
                           "mcu:    {mcu}\\r\\n";
"""

PLATFORMIO_SCONSSCRIPT_FMT = """#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2014-2017, Erik Moqvist
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# This file is part of the Simba project.
#

import os
from os.path import join
import subprocess
import shutil

from SCons.Script import DefaultEnvironment
from platformio.builder.tools.platformio import SRC_DEFAULT_FILTER

# Map the PlatformIO board name to the Simba board name
BOARD_MAP = {{
    "due": "arduino_due",
    "megaatmega2560": "arduino_mega",
    "nanoatmega328": "arduino_nano",
    "uno": "arduino_uno",
    "nodemcuv2": "nodemcu",
    "esp32dev": "esp32_devkitc",
    "d1_mini": "wemos_d1_mini"
}}

# Map the PlatformIO board name to the Simba board name
SUPPORTED_BOARDS = [
    "arduino_due",
    "arduino_mega",
    "arduino_nano",
    "arduino_uno",
    "esp12e",
    "esp01",
    "nodemcu",
    "huzzah",
    "nano32",
    "esp32_devkitc"
]


BOARDS = {boards}


def add_include_paths(env, paths):
    \"\"\"Add given list of include paths.

    \"\"\"

    platform = env.PioPlatform()
    env.Append(CPPPATH=["$PROJECTSRC_DIR"])
    framework_dir = platform.get_package_dir("framework-simba")
    assert os.path.isdir(framework_dir)

    for path in paths:
        env.Append(CPPPATH=[join(framework_dir, path)])


def set_default_values(env):
    \"\"\"Set default values in the environment.

    \"\"\"

    if "VERSION" not in env:
        with open(env.subst(join("$PLATFORMFW_DIR", "VERSION.txt"))) as f:
            env.Append(VERSION=f.read().strip())

    if "NAME" not in env:
        env.Append(NAME="app_name")


def setup_mcu_esp(env, linker_script, flash_size_map):
    \"\"\"Setup the ESP environment.

    \"\"\"

    builders = {{}}

    for key, value in env["BUILDERS"].items():
        if key not in ["ElfToBin"]:
            builders[key] = value

    def build_function(target, source, env):
        subprocess.check_call(
            "xtensa-lx106-elf-objcopy --only-section .text -O binary "
            + str(source[0]) + " eagle.app.v6.text.bin",
            shell=True)
        subprocess.check_call(
            "xtensa-lx106-elf-objcopy --only-section .data -O binary "
            + str(source[0]) + " eagle.app.v6.data.bin",
            shell=True)
        subprocess.check_call(
            "xtensa-lx106-elf-objcopy --only-section .rodata -O binary "
            + str(source[0]) + " eagle.app.v6.rodata.bin",
            shell=True)
        subprocess.check_call(
            "xtensa-lx106-elf-objcopy --only-section .irom0.text -O binary "
            + str(source[0]) + " eagle.app.v6.irom0text.bin",
            shell=True)
        subprocess.check_call(
            env.subst('$PYTHONEXE "$PLATFORMFW_DIR/3pp/ESP8266_RTOS_SDK/tools/gen_appbin.py"') + " " + str(source[0]) + " 2 0 0 " + flash_size_map,
            shell=True)
        shutil.copy("eagle.app.flash.bin", str(target[0]))

    builders["ElfToBin"] = env.Builder(
        action=build_function,
        suffix=".bin")

    env.Replace(BUILDERS=builders)

    env.Replace(UPLOADCMD=' '.join([
        '"$PYTHONEXE" "$PLATFORMFW_DIR/3pp/esptool/esptool.py"',
        '--baud 230400',
        '--port $UPLOAD_PORT',
        'write_flash',
        '0x00000 "$PLATFORMFW_DIR/3pp/ESP8266_RTOS_SDK/bin/boot_v1.4.bin"',
        '0x01000 "$BUILD_DIR/firmware.bin"']))



def create_src_filter(srcs):
    \"\"\"Create the source filter string.
    \"\"\"

    src_filter = [
        "-<bin/>",
        "-<3pp/>",
        "-<src/>",
        "-<tst/>"
    ]

    for src in srcs:
        src_filter.append("+<{{}}>".format(src))

    return SRC_DEFAULT_FILTER + " " + " ".join(src_filter)


def setup_mcu_esp(env, linker_script, flash_size_map):
    \"\"\"Setup the ESP environment.

    \"\"\"

    # Without those some applicaions do not start.
    linkflags = [
        "-Wl,--start-group",
        "-lhal",
        "-lgcc",
        "-lphy",
        "-lpp",
        "-lnet80211",
        "-lwpa",
        "-lcrypto",
        "-lmain",
        "-lfreertos",
        "-llwip",
        "-lminic",
        "-Wl,--end-group"
    ]

    for flag in env["LINKFLAGS"]:
        if "-Wl,-wrap,register_chipv6_phy" in flag:
            continue
        linkflags.append(flag)
    env.Replace(LINKFLAGS=linkflags)
    env.Replace(LDSCRIPT_PATH="script.ld")

    env.Append(
        BUILDERS=dict(
            ElfToBin=Builder(
                action=" ".join([
                    '"$OBJCOPY"',
                    "-eo",
                    '"%s"' % join("$PLATFORMFW_DIR", "3pp", "esp8266Arduino",
                                  "2.3.0", "bootloaders", "eboot", "eboot.elf"),
                    "-bo", "$TARGET",
                    "-bm", "$BOARD_FLASH_MODE",
                    "-bf", "${{__get_board_f_flash(__env__)}}",
                    "-bz", "${{__get_flash_size(__env__)}}",
                    "-bs", ".text",
                    "-bp", "4096",
                    "-ec",
                    "-eo", "$SOURCES",
                    "-bs", ".irom0.text",
                    "-bs", ".text",
                    "-bs", ".data",
                    "-bs", ".rodata",
                    "-bc", "-ec"
                ]),
                suffix=".bin"
            )
        )
    )


def setup_board_esp12e(env):
    \"\"\"Setup the ESP12E environment.

    \"\"\"

    setup_mcu_esp(env, "simba.flash.4m.ld", "6")


def setup_board_esp01(env):
    \"\"\"Setup the ESP01 environment.

    \"\"\"

    setup_mcu_esp(env, "simba.flash.1m.ld", "2")


def setup_board_nodemcu(env):
    \"\"\"Setup the NodeMCU environment.

    \"\"\"

    setup_mcu_esp(env, "simba.flash.4m.ld", "6")


def setup_board_huzzah(env):
    \"\"\"Setup the Huzzah environment.

    \"\"\"

    setup_mcu_esp(env, "simba.flash.4m.ld", "6")


def setup_board_wemos_d1_mini(env):
    \"\"\"Setup the WEMOS D1 mini environment.

    \"\"\"

    setup_mcu_esp(env, "simba.flash.4m.ld", "6")


def setup_board_nano32(env):
    \"\"\"Setup the Nano32 environment.

    \"\"\"

    ccflags = []

    for flag in env["CCFLAGS"]:
        if "-Wpointer-arith" in flag:
            continue
        ccflags.append(flag)
    env.Replace(CCFLAGS=ccflags)

    cppdefines = []

    for cppdefine in env["CPPDEFINES"]:
        try:
            if "MBEDTLS" in cppdefine[0]:
                continue
        except:
            pass
        cppdefines.append(cppdefine)
    env.Replace(CPPDEFINES=cppdefines)

    # Backwards compatibility for uploader flags still in platformio
    # repository.
    uploaderflags = []

    for value in env["UPLOADERFLAGS"]:
        if value.startswith('0x'):
            break
        uploaderflags.append(value)
    env.Replace(UPLOADERFLAGS=uploaderflags)

    env.Append(
        UPLOADERFLAGS=[
            "0x1000", join("$PLATFORMFW_DIR", "3pp", "esp32",
                           "bin", "bootloader.bin"),
            "0x8000", join("$PLATFORMFW_DIR", "3pp", "esp32",
                           "bin", "partitions_singleapp.bin"),
            "0x10000"
        ]
    )


env = DefaultEnvironment()

set_default_values(env)

# Rename the name of boards that have different name in PlatformIO and
# Simba.
if env["BOARD"] in BOARD_MAP:
    board = BOARD_MAP[env["BOARD"]]
else:
    board = env.subst(env["BOARD"])

if board not in SUPPORTED_BOARDS:
   raise ValueError("BOARD {{}} is not supported by Simba.".format(board))

# Add the default configuration for the board.
add_include_paths(env, BOARDS[board]["inc"])
env.Append(CPPDEFINES=BOARDS[board]["cdefs"])
src_filter = create_src_filter(BOARDS[board]["src"])
env.Replace(CFLAGS=BOARDS[board]["cflags"])
env.Replace(CXXFLAGS=BOARDS[board]["cxxflags"])
env.Replace(LINKFLAGS=BOARDS[board]["ldflags"])
env.Replace(LIBPATH=[os.path.join("$PLATFORMFW_DIR", path)
                     for path in BOARDS[board]["libpath"]])
env.Replace(MCU_DESC=BOARDS[board]["mcu_desc"])
env.Replace(BOARD_DESC=BOARDS[board]["board_desc"])
env.Append(LIBS=BOARDS[board]["lib"])

# Always replace the map file path.
linkflags = []

for flag in env["LINKFLAGS"]:
    if flag.startswith("-Wl,-Map="):
        flag = '-Wl,-Map="$BUILD_DIR/firmware.map"'
    linkflags.append(flag)
env.Replace(LINKFLAGS=linkflags)

# For some boards the configuration has to be modified.
if board == "arduino_due":
    linkflags = []

    for flag in env["LINKFLAGS"]:
        if flag.startswith("-T"):
            continue
        linkflags.append(flag)
    env.Replace(LINKFLAGS=linkflags)
    env.Replace(LDSCRIPT_PATH="script.ld")
elif board == "esp12e":
    setup_board_esp12e(env)
elif board == "esp01":
    setup_board_esp01(env)
elif board == "nodemcu":
    setup_board_nodemcu(env)
elif board == "huzzah":
    setup_board_huzzah(env)
elif board == "wemos_d1_mini":
    setup_board_wemos_d1_mini(env)
elif board == "nano32":
    setup_board_nano32(env)
elif board == "esp32_devkitc":
    setup_board_nano32(env)
elif 'ARCH_AVR' in BOARDS[board]['cdefs']:
    # LTO makes the application crash.
    linkflags = []
    for flag in env["LINKFLAGS"]:
        if flag == "-flto":
            continue
        linkflags.append(flag)
    env.Replace(LINKFLAGS=linkflags)

    ccflags = []
    for flag in env["CCFLAGS"]:
        if flag == "-flto":
            continue
        ccflags.append(flag)
    env.Replace(CCFLAGS=ccflags)

# generated files
SIMBA_GEN_C = "$BUILD_DIR/SimbaFramework/simba_gen.c"

# create a list of all sources
variant_dir = join("$BUILD_DIR", "SimbaFramework")
src_dir = join("$PLATFORMFW_DIR")

source_files = []

for src in env.LookupSources(variant_dir, src_dir, True, src_filter):
    source_files.append(env.Object(src))

# Command to generate simba_gen.c
env.Command(SIMBA_GEN_C,
            source_files,
            ('"$PYTHONEXE" "$PLATFORMFW_DIR/bin/simbagen.py" '
             '--output-directory "$BUILD_DIR/SimbaFramework" '
             'source '
             '--name "$NAME" '
             '--version "$VERSION" '
             '--board "$BOARD_DESC" '
             '--mcu "$MCU_DESC" '
             '--endianess little '
             '--eeprom-soft-chunk-size 2048 '))
source_files.append(SIMBA_GEN_C)

lib = env.Library(target=join("$BUILD_DIR", "SimbaFramework"), source=source_files)

env.Append(LIBS=[lib])
"""


def create_database():
    """Generate the simba database with baord and mcu information.

    """

    return json.loads(subprocess.check_output(["dbgen.py"]))


def generate_platformio_sconsscript(database, version):
    """Generate the platformio scons script.

    """

    simba_root = os.environ["SIMBA_ROOT"]
    boards = {}

    # Only add selceted parts the database to the scons script for
    # less unnecessary information.
    for board, data in database["boards"].items():
        # Add everything we need, and a little more.
        selected_data = {
            'inc': data['inc'],
            'cdefs': [re.sub(r'"(.*)"', r'"\"\1\""', cdef)
                      for cdef in data['cdefs']
                      if not cdef.startswith("F_CPU")],
            'src': data['src'],
            'cflags': data['cflags'],
            'cxxflags': data['cxxflags'],
            'libpath': data['libpath'],
            'ldflags': data['ldflags'],
            'linker_script': data['linker_script'],
            'lib': data['lib'],
            'board_desc': data['board_desc'],
            'mcu_desc': database['mcus'][data['mcu']]['mcu_desc']
        }
        boards[board] = selected_data

    outfile = os.path.join(simba_root, "make", "platformio.sconscript")
    with open(outfile, "w") as fout:
        fout.write(PLATFORMIO_SCONSSCRIPT_FMT.format(
            boards=json.dumps(boards, indent=4, sort_keys=True)))


def main():
    """Create the platformio build script.

    """

    parser = argparse.ArgumentParser()
    parser.add_argument("--version", required=True)
    args = parser.parse_args()

    print("Creating software database.")
    database = create_database()

    generate_platformio_sconsscript(database, args.version)


if __name__ == "__main__":
    main()
