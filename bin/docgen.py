#!/usr/bin/env python3

"""Generate documentation.

"""

from __future__ import print_function

import os
import argparse
import json
import subprocess
import glob


SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
BOARD_TEMPLATE_RST_FMT = os.path.join(SCRIPT_DIR,
                                      'docgen',
                                      'board-template.rst')

CONFIG_FMT = """|  {:53} |  {:50} |
+--------------------------------------------------------+-----------------------------------------------------+
"""


SOURCE_CODE_FMT = """.. code-block:: c

{source}
"""


def write_to_file(path, data):
    print("Writing to {}... ".format(path), end='')

    with open(path, "w") as fout:
        fout.write(data)

    print("done.")


def generate_drivers(config):
    drivers = []

    for driver in sorted(config):
        subsystem = glob.glob('src/drivers/*/' + driver + '.h')[0].split('/')[2]
        drivers.append("- :doc:`../library-reference/drivers/{}/{}`".format(
            subsystem,
            driver))

    return '\n'.join(drivers)


def generate_include_extra(board):
    if os.path.exists(os.path.join("doc", "boards", "extra", board + ".rst")):
        return ".. include:: extra/{}.rst".format(board)
    else:
        return ""


def generate_major_features(default_configuration):
    major_features = []

    for name, value in default_configuration:
        if name == "CONFIG_START_NETWORK" and value == "1":
            major_features.append("- Networking.")

        if name == "CONFIG_START_FILESYSTEM" and value == "1":
            major_features.append("- File system.")

        if name == "CONFIG_START_CONSOLE" and value != "CONFIG_START_CONSOLE_NONE":
            major_features.append("- :doc:`Console.<../library-reference/oam/console>`")

        if name == "CONFIG_START_SHELL" and value == "1":
            major_features.append(
                "- :doc:`Debug shell.<../library-reference/oam/shell>`")

    return '\n'.join(major_features)


def generate_console_params(default_configuration):
    console_params = {}

    for name, value in default_configuration:
        if name == "CONFIG_START_CONSOLE":
            if value == "CONFIG_START_CONSOLE_UART":
                console_params["channel"] = "UART"
            elif value == "CONFIG_START_CONSOLE_USB_CDC":
                console_params["channel"] = "USB"
                console_params["settings"] = ""
            else:
                console_params["channel"] = "unknown"
                console_params["settings"] = ""

        # The communication channel parameters (nothing for USB).
        if ((name == "CONFIG_START_CONSOLE_UART_BAUDRATE")
            and (console_params["channel"] == "UART")):
            console_params["settings"] = " {}-8-N-1".format(value)

    return console_params


def generate_memory_usage(board):
    applications = [
        "minimal-configuration",
        "default-configuration"
    ]
    memory_usage = []

    try:
        for application in applications:
            subprocess.check_call([
                'make',
                '-s',
                '-C', os.path.join('examples', application),
                'BOARD=' + board,
                'all'
            ])
            sizes_json = subprocess.check_output([
                'make',
                '-s',
                '-C', os.path.join('examples', application),
                'BOARD=' + board,
                'size-json'
            ],
            universal_newlines=True)
            sizes = json.loads(sizes_json)
            fmt = '| {application:24} | {program:9} | {data:9} |'
            memory_usage.append(fmt.format(application=application,
                                           program=sizes['program'],
                                           data=sizes['data']))
    except subprocess.CalledProcessError:
        print(
            'Failed to generate memory footprint data for board {}.'.format(
                board))

    separator = '\n+-{}-+-----------+-----------+\n'.format(24 * '-')

    return separator.join(memory_usage)


def generate_default_configuration(config):
    default_configuration = []
    targets = []

    for name, value in config:
        default_configuration.append(CONFIG_FMT.format(name + "_", value))
        target = ".. _{name}: ../user-guide/configuration.html#c.{name}".format(
            name=name)
        targets.append(target)

    return ''.join(default_configuration), '\n\n'.join(targets)


def generate_boards(database):
    """Generate documentation for all boards.

    """

    with open(BOARD_TEMPLATE_RST_FMT, 'r') as fin:
        board_rst_fmt = fin.read()

    for board, data in database["boards"].items():
        config = data["default-configuration"]
        drivers = generate_drivers(data["drivers"])
        include_extra = generate_include_extra(board)
        default_configuration, targets = generate_default_configuration(config)
        major_features = generate_major_features(config)
        console_params = generate_console_params(config)
        memory_usage = generate_memory_usage(board)

        rst = board_rst_fmt.format(
            name=board,
            desc=data["board_desc"],
            desc_underline="=" * len(data["board_desc"]),
            homepage=data["board_homepage"],
            pinout=data["board_pinout"],
            major_features=major_features,
            mcu=data["mcu"].replace("/", ""),
            drivers=drivers,
            default_configuration=default_configuration,
            include_extra=include_extra,
            targets=targets,
            memory_usage=memory_usage,
            console_params=console_params)

        rst_path = os.path.join("doc", "boards", board + ".rst")

        write_to_file(rst_path, rst)


def generate_examples():
    """Generate the examples.

    """

    examples = [
        "analog_read",
        "analog_write",
        "blink",
        "ds18b20",
        "dht",
        "filesystem",
        "hello_world",
        "http_client",
        "ping",
        "queue",
        "shell",
        "timer"
    ]

    for example in examples:
        c_path = os.path.join("examples", example, "main.c")
        source = []

        with open(c_path) as fin:
            for line in fin.readlines():
                source.append("   " + line)

        rst = SOURCE_CODE_FMT.format(source=''.join(source))
        rst_path = os.path.join("doc", "examples", example, "source-code.rst")

        write_to_file(rst_path, rst)


def generate_testing(database):
    """Generate the list of test suites.

    """

    testing_suites_path = os.path.join("doc",
                                       "developer-guide",
                                       "testing-suites.rst")

    with open(testing_suites_path, "w") as fout:
        boards = sorted(database["boards"].keys())

        for board in boards:
            suites = subprocess.check_output([
                'make',
                '-s',
                'BOARD=' + board,
                'print-TESTS'
            ],
            universal_newlines=True)
            print(database["boards"][board]["board_desc"], file=fout)
            print('-' * len(board), file=fout)
            print(file=fout)

            for suite in suites.split(" "):
                suite = suite[4:].strip()

                if suite:
                    fmt = "- :github-blob:`{suite}<tst/{suite}/main.c>`"
                    print(fmt.format(suite=suite), file=fout)

            print(file=fout)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("database",
                        help="JSON database.")
    args = parser.parse_args()

    with open(args.database) as fin:
        database = json.load(fin)

    generate_boards(database)
    generate_examples()
    generate_testing(database)


if __name__ == "__main__":
    main()
