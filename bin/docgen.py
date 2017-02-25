#!/usr/bin/env python

"""Generate documentation.
"""

from __future__ import print_function

import os
import argparse
import json
import subprocess


BOARD_FMT = """{desc}
{desc_underline}

Pinout
------

.. image:: ../images/boards/{pinout}
   :width: 50%
   :target: ../_images/{pinout}

{include_extra}

Default system features
-----------------------

The default configuration includes those major features. They are all
initialized by ``sys_start()`` at the startup of the application.

{major_features}

Drivers
-------

Supported drivers for this board.

{drivers}

Library Reference
-----------------

Read more about board specific functionality in the :doc:`{desc}
<../library-reference/boards/{name}>` module documentation in the
Library Reference.

Memory usage
------------

Below is the memory usage of two applications:

- The
  :github-tree:`minimal-configuration<examples/minimal-configuration>`
  application is configured to only include the bare minimum of
  functionality for the low level kernel to run. That is, the
  thread scheduler and system tick.

- The
  :github-tree:`default-configuration<examples/default-configuration>`
  application is built with the default configuration, including a lot
  more functionality. See the list of `Default system features`_ above
  for a summary.

+--------------------------+-----------+-----------+
| Application              | Flash     | RAM       |
+==========================+===========+===========+
{memory_usage}
+--------------------------+-----------+-----------+

Default configuration
---------------------

Default Standard Library configuration.

+--------------------------------------------------------+-----------------------------------------------------+
|  Name                                                  |  Value                                              |
+========================================================+=====================================================+
{default_configuration}

Homepage
--------

{homepage}

Mcu
---

:doc:`{mcu}<../library-reference/mcus/{mcu}>`

{targets}
"""


CONFIG_FMT = """|  {:53} |  {:50} |
+--------------------------------------------------------+-----------------------------------------------------+
"""


SOURCE_CODE_FMT = """.. code-block:: c

{source}
"""


def get_arguments():
    """Get the command line arguments.

    """

    parser = argparse.ArgumentParser()

    parser.add_argument("database",
                        help="JSON database.")

    return parser.parse_args()


def boards_generate(database):
    """Generate boards.

    """

    for board, data in database["boards"].items():
        # Board drivers.
        drivers = []
        for driver in sorted(data["drivers"]):
            drivers.append("- :doc:`../library-reference/drivers/{}`".format(
                driver))

        targets = []

        # Default configuration.
        default_configuration = ""
        for config in data["default-configuration"]:
            default_configuration += CONFIG_FMT.format(config[0] + "_", config[1])
            target = ".. _{name}: ../user-guide/configuration.html#c.{name}".format(
                name=config[0])
            targets.append(target)

        if os.path.exists(os.path.join("doc", "boards", "extra", board + ".rst")):
            include_extra = ".. include:: extra/{name}.rst".format(name=board)
        else:
            include_extra = ""

        # Enabled features.
        major_features = []
        for [name, value] in data["default-configuration"]:
            if name == "CONFIG_START_NETWORK" and value == "1":
                major_features.append("- Networking.")
            if name == "CONFIG_START_FILESYSTEM" and value == "1":
                major_features.append("- File system.")
            if name == "CONFIG_START_CONSOLE" and value != "CONFIG_START_CONSOLE_NONE":
                major_features.append("- :doc:`Console.<../library-reference/oam/console>`")
            if name == "CONFIG_START_SHELL" and value == "1":
                major_features.append("- :doc:`Debug shell.<../library-reference/oam/shell>`")

        # Memory usage.
        applications = [
            "minimal-configuration",
            "default-configuration"
        ]
        memory_usage = []

        try:
            for application in applications:
                subprocess.check_call(['make',
                                       '-s',
                                       '-C', os.path.join('examples', application),
                                       'BOARD=' + board,
                                       'all'])
                sizes_json = subprocess.check_output(['make',
                                                      '-s',
                                                      '-C', os.path.join('examples', application),
                                                      'BOARD=' + board,
                                                      'size-json'])
                sizes = json.loads(sizes_json)
                memory_usage.append('| {application:24} | {program:9} | {data:9} |'.format(
                    application=application,
                    program=sizes['program'],
                    data=sizes['data']))
        except subprocess.CalledProcessError:
            print('Failed to generate memory footprint data for board {}. '
                  'Skipping board.'.format(board))
            continue

        rst = BOARD_FMT.format(name=board,
                               desc=data["board_desc"],
                               desc_underline="=" * len(data["board_desc"]),
                               homepage=data["board_homepage"],
                               pinout=data["board_pinout"],
                               major_features='\n'.join(major_features),
                               mcu=data["mcu"].replace("/", ""),
                               drivers='\n'.join(drivers),
                               default_configuration=default_configuration,
                               include_extra=include_extra,
                               targets='\n\n'.join(targets),
                               memory_usage='\n+-{}-+-----------+-----------+\n'.format(
                                   24 * '-').join(memory_usage))

        rst_path = os.path.join("doc", "boards", board + ".rst")
        print("Writing to ", rst_path)
        with open(rst_path, "w") as fout:
            fout.write(rst)


def examples_generate(_):
    """Generate examples.

    """

    examples = [
        "analog_read",
        "analog_write",
        "blink",
        "ds18b20",
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

        print("Writing to ", rst_path)
        with open(rst_path, "w") as fout:
            fout.write(rst)


def testing_generate(database):
    """Generate the list of test suites.

    """

    testing_suites_path = os.path.join("doc", "developer-guide", "testing-suites.rst")

    with open(testing_suites_path, "w") as fout:
        boards = database["boards"].keys()
        boards.sort()
        for board in boards:
            suites = subprocess.check_output(['make',
                                              '-s',
                                              'BOARD=' + board,
                                              'print-TESTS'])
            print(database["boards"][board]["board_desc"], file=fout)
            print('-' * len(board), file=fout)
            print(file=fout)
            for suite in suites.split(" "):
                suite = suite[4:].strip()
                if suite:
                    print("- :github-blob:`{suite}<tst/{suite}/main.c>`".format(suite=suite),
                          file=fout)
            print(file=fout)


def main():
    """Main.

    """

    args = get_arguments()

    with open(args.database) as fin:
        database = json.load(fin)

    boards_generate(database)
    examples_generate(database)
    testing_generate(database)


if __name__ == "__main__":
    main()
