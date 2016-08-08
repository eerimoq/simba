#!/usr/bin/env python

from __future__ import print_function

import subprocess
import argparse
import json
import errno
import os
import shutil
import fnmatch
import hashlib


ARDUINO_H = """/**
 * This is a generated file required by the Arduino build system."
 */

#include "simba.h"
"""

SIMBA_GEN_C_FMT = """
#include "simba.h"

const FAR char sysinfo[] = "app:    myapp built - by -.\\r\\n"
                           "board:  {board}\\r\\n"
                           "mcu:    {mcu}\\r\\n";
"""

PLATFORMIO_SCONSSCRIPT_FMT = """#
# @file make/platformio.sconscript
# @version {version}
#
# @section License
# Copyright (C) 2014-2016, Erik Moqvist
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
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
    "uno": "arduino_uno"
}}


def add_include_paths(env, paths):
    \"\"\"Add given list of include paths.

    \"\"\"

    for path in paths:
        env.VariantDirWrap(
            join("$BUILD_DIR", "SimbaFramework", path),
            join("$PLATFORMFW_DIR", path)
        )
        env.Append(CPPPATH=[join("$BUILD_DIR", "SimbaFramework", path)])


def add_common_include_paths(env):
    \"\"\"Add the common include paths.

    \"\"\"

    env.Append(CPPPATH=[join("$BUILD_DIR", "SimbaFramework")])
    env.Append(CPPPATH=[join("$BUILD_DIR", "SimbaFramework", "src")])

    paths = [
        "src",
        "gen",
        join("src", "boards", "$BOARD"),
        join("src", "mcus", "$BOARD_MCU"),
        join("src", "kernel", "ports", "$ARCH", "gnu"),
        join("src", "drivers", "ports", "$FAMILY"),
        join("src", "inet")
    ]

    add_include_paths(env, paths)


def add_common_defines(env):
    \"\"\"Add common defines.

    \"\"\"

    env.Append(
        CPPDEFINES=[
            "ARCH_$ARCH_UPPER",
            "FAMILY_$FAMILY_UPPER",
            "MCU_$MCU_UPPER",
            "BOARD_$BOARD_UPPER",
            "VERSION=$VERSION"
        ]
    )


def set_default_values(env):
    \"\"\"Set default values in the environment.

    \"\"\"

    if "VERSION" not in env:
        with open(env.subst(join("$PLATFORMFW_DIR", "VERSION.txt"))) as f:
            env.Append(VERSION=f.read().strip())

    if "NAME" not in env:
        env.Append(NAME="app_name")


def setup_mcu_avr(env):
    \"\"\"Setup the AVR MCU environment.

    \"\"\"

    if env.subst(env["BOARD_MCU"]) == "atmega2560":
        env.Replace(MCU_DESC="Atmel ATMega25600 AVR @ 16MHz, 8k sram, 256k flash",
                    MAIN_STACK_END="0x802200")
    elif env.subst(env["BOARD_MCU"]) == "atmega328p":
        env.Replace(MCU_DESC="Atmel ATMega328p AVR @ 16MHz, 2k sram, 32k flash",
                    MAIN_STACK_END="0x800900")
    else:
        env.Replace(MCU_DESC="Not found")

    env.Replace(ARCH="avr",
                FAMILY="avr",
                SETTING_MEMORY="eeprom",
                SETTING_OFFSET="128",
                SETTING_SIZE="256",
                ENDIANESS="little")

    add_include_paths(env, [
        join("3pp", "lwip-1.4.1", "src", "include"),
        join("3pp", "lwip-1.4.1", "src", "include", "ipv4")
    ])

    env.Append(CCFLAGS=[
        "-funsigned-bitfields",
        "-std=gnu99",
        "-Wno-error=unused-variable",
        "-Werror",
        "-O2",
        "-fpack-struct"
    ])

    env.Append(LINKFLAGS=[
        "-Wl,--defsym=__main_stack_end=$MAIN_STACK_END",
        "-Wl,-Map=$BUILD_DIR/firmware.map", # This should be added to PlatformIO
    ])


def setup_mcu_sam(env):
    \"\"\"Setup the SAM MCU environment.

    \"\"\"

    if env.subst(env["BOARD_MCU"]).endswith("/3x8e"):
        env.Replace(MCU_DESC="Atmel SAM3X8E Cortex-M3 @ 84MHz, 96k sram, 512k flash")
    else:
        env.Replace(MCU_DESC="Not found")

    env.Replace(ARCH="arm",
                FAMILY="sam",
                SETTING_MEMORY="flash",
                SETTING_OFFSET="0",
                SETTING_SIZE="4096",
                ENDIANESS="little")

    add_include_paths(env, [
        join("3pp", "lwip-1.4.1", "src", "include"),
        join("3pp", "lwip-1.4.1", "src", "include", "ipv4")
    ])

    env.Append(CCFLAGS=[
        "-funsigned-bitfields",
        "-std=gnu99",
        "-Wno-error=unused-variable",
        "-Werror",
        "-O2",
        "-fpack-struct"
    ])

    linkflags = []

    for flag in env["LINKFLAGS"]:
        if "-Wl,--entry" not in flag:
            linkflags.append(flag)

    env.Replace(LINKFLAGS=linkflags + [
        "-Wl,-T$PLATFORMFW_DIR/src/mcus/$BOARD_MCU/script.ld",
        "-Wl,-Map=$BUILD_DIR/firmware.map" # This should be added to PlatformIO
    ])

    env.Append(LIBPATH=["$PLATFORMFW_DIR/src/mcus/$BOARD_MCU"])


def setup_mcu_esp(env, linker_script, flash_size_map):
    \"\"\"Setup the ESP environment.

    \"\"\"

    env.Replace(ARCH="esp",
                FAMILY="esp",
                SETTING_MEMORY="flash",
                SETTING_OFFSET="128",
                SETTING_SIZE="256",
                ENDIANESS="little")

    add_include_paths(env, [
        "src/mcus/esp8266",
        "3pp/ESP8266_RTOS_SDK/extra_include",
        "3pp/ESP8266_RTOS_SDK/include",
        "3pp/ESP8266_RTOS_SDK/include/espressif",
        "3pp/ESP8266_RTOS_SDK/include/espressif/esp8266",
        "3pp/ESP8266_RTOS_SDK/include/lwip",
        "3pp/ESP8266_RTOS_SDK/include/lwip/ipv4",
        "3pp/ESP8266_RTOS_SDK/include/lwip/ipv6"
    ])

    linkflags = []

    for flag in env["LINKFLAGS"]:
        if ("-Wl,-T" not in flag
            and "-Wl,-wrap,register_chipv6_phy" not in flag):
            linkflags.append(flag)

    env.Replace(LINKFLAGS=linkflags + [
        "-Wl,--start-group",
        "-lminic",
        "-lgcc",
        "-lhal",
        "-lphy",
        "-lpp",
        "-lnet80211",
        "-lwpa",
        "-lcrypto",
        "-lmain",
        "-lfreertos",
        "-llwip",
        "-Wl,--end-group",
        "-Wl,-T" + linker_script
    ])

    env.Append(LIBPATH=[
        "$PLATFORMFW_DIR/src/mcus/$BOARD_MCU/ld",
        "$PLATFORMFW_DIR/3pp/ESP8266_RTOS_SDK/lib"
    ])

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
            env.subst('$PYTHONEXE $PLATFORMFW_DIR/3pp/ESP8266_RTOS_SDK/tools/gen_appbin.py') + " " + str(source[0]) + " 2 0 0 " + flash_size_map,
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
        '0x00000 $PLATFORMFW_DIR/3pp/ESP8266_RTOS_SDK/bin/boot_v1.4.bin',
        '0x01000 $BUILD_DIR/firmware.bin']))


def setup_board_arduino_due(env):
    \"\"\"Setup the Arduino Due environment.

    \"\"\"

    add_include_paths(env, ["src/mcus/sam"])
    setup_mcu_sam(env)


def setup_board_arduino_mega(env):
    \"\"\"Setup the Arduino Mega environment.

    \"\"\"

    setup_mcu_avr(env)


def setup_board_arduino_uno(env):
    \"\"\"Setup the Arduino Uno environment.

    \"\"\"

    setup_mcu_avr(env)


def setup_board_arduino_nano(env):
    \"\"\"Setup the Arduino Nano environment.

    \"\"\"

    setup_mcu_avr(env)


def setup_board_esp12e(env):
    \"\"\"Setup the ESP12E environment.

    \"\"\"

    setup_mcu_esp(env, "simba.flash.4m.ld", "6")


def setup_board_esp01(env):
    \"\"\"Setup the ESP01 environment.

    \"\"\"

    setup_mcu_esp(env, "simba.flash.1m.ld", "2")


def create_src_filter(env):
    \"\"\"Create the source filter string.
    \"\"\"

    src_filter_patterns = []

    # Add the board files.
    src_filter_patterns.append("-<src/boards>")
    src_filter_patterns.append(env.subst("+<src/boards/$BOARD>"))

    # Add the mcu files.
    src_filter_patterns.append("-<src/mcus>")
    src_filter_patterns.append(env.subst("+<src/mcus/$BOARD_MCU>"))

    if env.subst("$BOARD_MCU") == "sam3x8e":
        src_filter_patterns.append("+<src/mcus/sam>")

    # Add the drivers files.
    src_filter_patterns.append("-<src/drivers>")

    src_filter_patterns.append("-<3pp/>")

    src_filter_patterns.append("-<rost/>")

    src_filter_patterns.append("-<tst/>")

    arch = env.subst("$ARCH")

    if arch == "arm":
        files = [
            "adc.c",
            "can.c",
            "chipid.c",
            "dac.c",
            "exti.c",
            "flash.c",
            "mcp2515.c",
            "pin.c",
            "sd.c",
            "spi.c",
            "uart.c",
            "usb.c",
            "usb_host.c",
            "usb/host/class/usb_host_class_hid.c",
            "usb/host/class/usb_host_class_mass_storage.c"
        ]

    elif arch == "avr":
        files = [
            "adc.c",
            "ds18b20.c",
            "ds3231.c",
            "exti.c",
            "i2c.c",
            "mcp2515.c",
            "nrf24l01.c",
            "spi.c",
            "owi.c",
            "pin.c",
            "pwm.c",
            "sd.c",
            "uart.c",
            "uart_soft.c"
        ]

    elif arch == "esp":
        files = [
            "adc.c",
            "pin.c",
            "spi.c",
            "uart.c"
        ]

    else:
        raise ValueError("bad architecture {{}}".format())

    for src in files:
        src_filter_patterns.append("+<src/drivers/" + src + ">")

    if arch != "esp":
        files = [
            "core/stats.c",
            "core/tcp_out.c",
            "core/udp.c",
            "core/timers.c",
            "core/netif.c",
            "core/def.c",
            "core/raw.c",
            "core/dns.c",
            "core/tcp_in.c",
            "core/memp.c",
            "core/pbuf.c",
            "core/tcp.c",
            "core/init.c",
            "core/dhcp.c",
            "core/ipv4/ip_frag.c",
            "core/ipv4/ip.c",
            "core/ipv4/ip_addr.c",
            "core/ipv4/icmp.c",
            "core/ipv4/igmp.c",
            "core/ipv4/inet.c",
            "core/ipv4/inet_chksum.c",
            "core/mem.c",
            "netif/etharp.c",
            "netif/ethernetif.c",
            "api/tcpip.c"
        ]

        for src in files:
            src_filter_patterns.append("+<3pp/lwip-1.4.1/src/" + src + ">")

    return SRC_DEFAULT_FILTER + " " + " ".join(src_filter_patterns)


env = DefaultEnvironment()

set_default_values(env)

# Rename the name of boards that have different name in PlatformIO and
# Simba.
if env["BOARD"] in BOARD_MAP:
    env["BOARD"] = BOARD_MAP[env["BOARD"]]

board = env.subst(env["BOARD"])

add_common_include_paths(env)
add_common_defines(env)

if board == "arduino_due":
    setup_board_arduino_due(env)
elif board == "arduino_mega":
    setup_board_arduino_mega(env)
elif board == "arduino_uno":
    setup_board_arduino_uno(env)
elif board == "arduino_nano":
    setup_board_arduino_nano(env)
elif board == "esp12e":
    setup_board_esp12e(env)
elif board == "esp01":
    setup_board_esp01(env)
else:
    raise ValueError("BOARD {{}} is not supported by Simba.".format(board))

# variables in upper case for defines
env.Append(ARCH_UPPER=env.subst(env["ARCH"]).upper().replace("/", "_"),
           FAMILY_UPPER=env.subst(env["FAMILY"]).upper().replace("/", "_"),
           MCU_UPPER=env.subst(env["BOARD_MCU"]).upper().replace("/", "_"),
           BOARD_UPPER=env.subst(env["BOARD"]).upper().replace("/", "_"))

if "SETTINGS_INI" not in env:
    env.Append(SETTINGS_INI=[join("$BUILD_DIR", "SimbaFramework", "make", "settings.ini")])
    env.VariantDirWrap(
        join("$BUILD_DIR", "SimbaFramework", "make"),
        join("$PLATFORMFW_DIR", "make")
    )

# generated files
SETTINGS_H = "$BUILD_DIR/SimbaFramework/gen/settings.h"
SETTINGS_C = env.subst("$BUILD_DIR/SimbaFramework/gen/settings.c")
SETTINGS_BIN = env.subst("$BUILD_DIR/SimbaFramework/gen/settings.bin")
SIMBA_GEN_C = "$BUILD_DIR/SimbaFramework/gen/simba_gen.c"

# generate settings
fmt = ('"$PYTHONEXE" "$PLATFORMFW_DIR/src/kernel/tools/settings.py" --{{filetype}} '
       "--output-directory $BUILD_DIR/SimbaFramework/gen --setting-memory $SETTING_MEMORY "
       "--setting-offset $SETTING_OFFSET --setting-size $SETTING_SIZE "
       "$SOURCE $ENDIANESS")
env.Command(SETTINGS_H, env["SETTINGS_INI"], fmt.format(filetype="header"))
env.Command(SETTINGS_C, env["SETTINGS_INI"], fmt.format(filetype="source"))
env.Command(SETTINGS_BIN, env["SETTINGS_INI"], fmt.format(filetype="binary"))

# create a list of all sources
variant_dir = join("$BUILD_DIR", "SimbaFramework")
src_dir = join("$PLATFORMFW_DIR")
src_filter = create_src_filter(env)

#print src_filter

source_files = []

for src in env.LookupSources(variant_dir, src_dir, True, src_filter):
#    print src
    source_files.append(env.Object(src))

# Command to generate simba_gen.c
env.Command(SIMBA_GEN_C,
            source_files,
            ('"$PYTHONEXE" "$PLATFORMFW_DIR/src/kernel/tools/gen.py" "$NAME" "$VERSION" '
             '"$BOARD_DESC" "$MCU_DESC" "$TARGET" $SOURCES'))
source_files.append(SIMBA_GEN_C)
source_files.append(SETTINGS_C)

lib = env.Library(target=join("$BUILD_DIR", "SimbaFramework"), source=source_files)
Depends(lib, SETTINGS_BIN)

env.Append(LIBS=[lib])
"""


def create_database():
    """Generate the simba database with baord and mcu information.

    """

    return json.loads(subprocess.check_output(["dbgen.py"]))


def generate_cores(family, database):
    """Generate the cores directory, shared among all boards.

    """

    cores_dir = os.path.join("cores", "simba")

    # Create the cores directory.
    mkdir_p(cores_dir)

    with open(os.path.join(cores_dir, "Arduino.h"), "w") as fout:
        fout.write(ARDUINO_H)

    simba_root = os.environ["SIMBA_ROOT"]
    cores_srcs = None

    # Generate a dummy settings files.
    for board in database["boards"].values():
        mcu = board["mcu"]

        if database["mcus"][mcu]["family"] != family:
            continue

        if cores_srcs is None:
            cores_srcs = set(board["src"])
        else:
            cores_srcs = cores_srcs & set(board["src"])

    if family == "avr":
        board = "arduino_pro_micro"
    elif family == "sam":
        board = "arduino_due"
    elif family == "esp":
        board = "esp01"
    else:
        raise ValueError("{}: bad family".format(family))

    # Copy all source files, except those in boards and mcus that are
    # variant specific. Use any board in given family
    for src in cores_srcs:
        dst_dir = os.path.join(cores_dir, os.path.dirname(src))
        mkdir_p(dst_dir)
        shutil.copy(os.path.join(simba_root, src), dst_dir)

    # Copy all header files.
    for inc in database["boards"][board]["inc"]:
        inc_dir = os.path.join(simba_root, inc)
        for root, _, filenames in os.walk(inc_dir):
            for filename in fnmatch.filter(filenames, '*.[hi]'):
                file_path = os.path.join(root, filename)
                file_dir = os.path.dirname(file_path)
                cores_file_dir = file_dir.replace(simba_root + "/", "")
                mkdir_p(os.path.join(cores_dir, cores_file_dir))
                shutil.copy(file_path,
                            os.path.join(cores_dir, cores_file_dir))

    # Various files.
    root_files = [
        "LICENSE",
        "README.rst",
        "VERSION.txt"
    ]

    for root_file in root_files:
        shutil.copy(os.path.join(simba_root, root_file), ".")

    return cores_srcs


def generate_variants(family, database, cores_srcs):
    """Generate the variants directory with board unique information.

    """

    simba_root = os.environ["SIMBA_ROOT"]

    print("Generating variants for family", family)

    for board_name, config in database['boards'].items():
        if database["mcus"][config["mcu"]]["family"] != family:
            continue

        variant_dir = os.path.join("variants", board_name)

        # Create the variant directory.
        mkdir_p(variant_dir)

        # Copy variant specific source files; those in "boards" and
        # "mcus". Other source files are copies in cores.
        for src in config["src"]:
            if src in cores_srcs:
                continue
            dst_dir = os.path.join(variant_dir, os.path.dirname(src))
            mkdir_p(dst_dir)
            shutil.copy(os.path.join(simba_root, src), dst_dir)

        setting_memory = config["setting_memory"]
        setting_offset = config["setting_offset"]
        setting_size = config["setting_size"]
        endianess = config["endianess"]

        subprocess.check_call([
            os.path.join(simba_root, "src/kernel/tools/settings.py"),
            "--header",
            "--output-directory", variant_dir,
            "--setting-memory", setting_memory,
            "--setting-offset", setting_offset,
            "--setting-size", setting_size,
            os.path.join(simba_root, "make/settings.ini"),
            endianess])

        subprocess.check_call([
            os.path.join(simba_root, "src/kernel/tools/settings.py"),
            "--source",
            "--output-directory", variant_dir,
            "--setting-memory", setting_memory,
            "--setting-offset", setting_offset,
            "--setting-size", setting_size,
            os.path.join(simba_root, "make/settings.ini"),
            endianess])

        # Copy all linker script files.
        for libpath in config["libpath"]:
            libpath_dir = os.path.join(simba_root, libpath)
            for root, _, filenames in os.walk(libpath_dir):
                for filename in fnmatch.filter(filenames, '*.ld'):
                    file_path = os.path.join(root, filename)
                    file_dir = os.path.dirname(file_path)
                    variant_file_dir = file_dir.replace(simba_root + "/",
                                                        "")
                    mkdir_p(os.path.join(variant_dir, variant_file_dir))
                    shutil.copy(file_path,
                                os.path.join(variant_dir, variant_file_dir))

        with open(os.path.join(variant_dir, "simba_gen.c"), "w") as fout:
            fout.write(SIMBA_GEN_C_FMT.format(name="my_app",
                                              board=board_name,
                                              mcu=config["mcu"]))


def generate_examples():
    """Generate the examples directory.

    libraries/Simba/examples/<example folder>

    """

    simba_root = os.environ["SIMBA_ROOT"]
    simba_examples_path = os.path.join(simba_root, 'examples')
    arduino_simba_path = os.path.join('libraries', 'Simba')
    arduino_examples_path = os.path.join(arduino_simba_path, 'examples')

    os.makedirs(arduino_examples_path)

    with open(os.path.join(arduino_simba_path, "Simba.h"), "w") as fout:
        fout.write("/* Generated file required by Arduino IDE. */")

    examples = [
        # example folder, sketch file
        ('blink', 'main.c'),
        ('hello_world', 'main.c'),
        ('shell', 'main.c')
    ]

    # Create the .ino-file.
    for example in examples:
        simba_example_path = os.path.join(simba_examples_path, example[0])
        arduino_example_path = os.path.join(arduino_examples_path, example[0])
        os.makedirs(arduino_example_path)
        shutil.copy(os.path.join(simba_example_path, example[1]),
                    os.path.join(arduino_example_path, example[0] + ".ino"))




def get_c_extra_flags(board, database):
    """Get include path, defines and flags to the compiler.

    """

    incs = database["boards"][board]["inc"]
    cdefs = database["boards"][board]["cdefs"]
    cflags = database["boards"][board]["cflags"]

    return " ".join(cflags
                    + ["\"-I{runtime.platform.path}/cores/simba/" + inc + "\""
                       for inc in incs]
                    + ["-D" + d for d in cdefs])


def get_c_elf_extra_flags(board, database):
    """Get library path, defines and flags to the linker.

    """

    libpaths = database["boards"][board]["libpath"]
    ldflags = database["boards"][board]["ldflags"]

    ldflags = [ldflag for ldflag in ldflags if "-Wl,-Map" not in ldflag]

    return " ".join(ldflags
                    + ["\"-L{runtime.platform.path}/variants/" + board + "/" + libpath + "\""
                       for libpath in libpaths])


def get_c_elf_extra_flags_after(board, database):
    """Get library path, defines and flags to the linker.

    """

    ldflags_after = database["boards"][board]["ldflags_after"]

    return " ".join(ldflags_after)


def generate_boards_txt_avr(database, boards_txt_fmt):
    """Generate boards.txt for AVR.

    """

    return boards_txt_fmt.format(
        mega2560_compiler_c_extra_flags=get_c_extra_flags("arduino_mega",
                                                          database),
        nano_compiler_c_extra_flags=get_c_extra_flags("arduino_nano",
                                                   database),
        uno_compiler_c_extra_flags=get_c_extra_flags("arduino_uno",
                                                   database),
        pro_micro_compiler_c_extra_flags=get_c_extra_flags("arduino_pro_micro",
                                                         database))


def generate_boards_txt_sam(database, boards_txt_fmt):
    """Generate boards.txt for SAM.

    """

    return boards_txt_fmt.format(
        arduino_due_x_dbg_compiler_c_extra_flags=get_c_extra_flags(
            "arduino_due",
            database),
        arduino_due_x_dbg_compiler_c_elf_extra_flags=get_c_elf_extra_flags(
            "arduino_due",
            database))


def generate_boards_txt_esp(database, boards_txt_fmt):
    """Generate boards.txt for ESP.

    """

    # ESP SDK libraries are copied to this location.
    libpath = "-L{runtime.platform.path}/lib"

    esp01_compiler_c_elf_extra_flags = get_c_elf_extra_flags("esp01", database)
    esp01_compiler_c_elf_extra_flags += " "
    esp01_compiler_c_elf_extra_flags += libpath

    esp12e_compiler_c_elf_extra_flags = get_c_elf_extra_flags("esp12e", database)
    esp12e_compiler_c_elf_extra_flags += " "
    esp12e_compiler_c_elf_extra_flags += libpath

    return boards_txt_fmt.format(
        esp01_compiler_c_extra_flags=get_c_extra_flags("esp01", database),
        esp01_compiler_c_elf_extra_flags=esp01_compiler_c_elf_extra_flags,
        esp01_compiler_c_elf_extra_flags_after=get_c_elf_extra_flags_after("esp01", database),
        esp12e_compiler_c_extra_flags=get_c_extra_flags("esp12e", database),
        esp12e_compiler_c_elf_extra_flags=esp12e_compiler_c_elf_extra_flags,
        esp12e_compiler_c_elf_extra_flags_after=get_c_elf_extra_flags_after("esp12e", database))


def generate_configuration_files(family, database):
    """Generate copy configuration files.

    """

    simba_root = os.environ["SIMBA_ROOT"]
    family_dir = os.path.join(simba_root,
                            "make",
                            "arduino",
                            family)
    configuration_files = [
        "platform.txt"
    ]

    for configuration_file in configuration_files:
        shutil.copy(os.path.join(family_dir, configuration_file), ".")

    with open("boards.txt", "w") as fout:
        with open(os.path.join(family_dir, "boards.txt"), "r") as fin:
            if family == "avr":
                boards_txt = generate_boards_txt_avr(database, fin.read())
            elif family == "sam":
                boards_txt = generate_boards_txt_sam(database, fin.read())
            elif family == "esp":
                boards_txt = generate_boards_txt_esp(database, fin.read())
            else:
                raise ValueError("Unsupported family {}.".format(family))

            fout.write(boards_txt)


def generate_extra(family, database):
    """Generate extra files that do not fit into any other generation
    function.

    """

    simba_root = os.environ["SIMBA_ROOT"]

    if family == "esp":
        # Copy all libraries.
        libpaths = database["boards"]["esp01"]["libpath"]
        mkdir_p("lib")

        for lib in database["boards"]["esp01"]["lib"]:
            for libpath in libpaths:
                libpath_dir = os.path.join(simba_root, libpath)
                for root, _, filenames in os.walk(libpath_dir):
                    for filename in filenames:
                        if filename != "lib" + lib + ".a":
                            continue
                        file_path = os.path.join(root, filename)
                        shutil.copy(file_path, "lib")
                        break

        # Copt eboot (bootloader).
        eboot_dir = os.path.join("bootloaders", "eboot")
        mkdir_p(eboot_dir)
        shutil.copy(os.path.join(simba_root,
                                 "3pp",
                                 "esp8266Arduino",
                                 "2.3.0",
                                 "bootloaders",
                                 "eboot",
                                 "eboot.elf"),
                    eboot_dir)


def generate_platformio_sconsscript(database, version):
    """Generate the platformio scons script.

    """

    simba_root = os.environ["SIMBA_ROOT"]

    #    for family in ["avr", "sam", "esp"]:
    #
    #        cores_srcs = generate_cores(family, database)
    #        generate_variants(family, database, cores_srcs)
    #        generate_examples()
    #        generate_configuration_files(family, database)
    #        generate_extra(family, database)
    outfile = os.path.join(simba_root, "make", "platformio.sconscript")
    with open(outfile, "w") as fout:
        fout.write(PLATFORMIO_SCONSSCRIPT_FMT.format(version=version))


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
