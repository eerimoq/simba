#!/usr/bin/env python

"""Generate documentation.
"""

import os
import argparse
import json


BOARD_FMT = """{desc}
{desc_underline}

Homepage
--------

{homepage}

Pinout
------

.. image:: ../images/boards/{pinout}
   :width: 50%
   :target: ../_images/{pinout}

Default system features
-----------------------

Here is a list of enabled higher level features for this board, all
initialized in ``sys_start()``:

{enabled_features}

Drivers
-------

Supported drivers.

{drivers}

Mcu
---

:doc:`{mcu}<../library-reference/mcus/{mcu}>`

Default configuration
---------------------

Default Standard Library configuration.

+------------------------------------------------------+-----------------------------------------------------+
|  Name                                                |  Value                                              |
+======================================================+=====================================================+
{default_configuration}

Library Reference
-----------------

Read more about board specific functionality in the :doc:`{desc}
<../library-reference/boards/{name}>` module documentation in the
Library Reference.

{include_extra}

{default_configuration_targets}
"""


CONFIG_FMT = """|  {:51} |  {:50} |
+------------------------------------------------------+-----------------------------------------------------+
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

        # Default configuration.
        default_configuration = ""
        default_configuration_targets = []
        for config in data["default-configuration"]:
            default_configuration += CONFIG_FMT.format(config[0] + "_", config[1])
            target = ".. _{name}: ../user-guide/configuration.html#c.{name}".format(
                name=config[0])
            default_configuration_targets.append(target)
            
        if os.path.exists(os.path.join("doc", "boards", "extra", board + ".rst")):
            include_extra = ".. include:: extra/{name}.rst".format(name=board)
        else:
            include_extra = ""

        # Enabled features.
        enabled_features = []
        for [name, value] in data["default-configuration"]:
            if name == "CONFIG_NETWORK_MANAGER" and value == "1":
                enabled_features.append("- Network manager.")
            if name == "CONFIG_FILESYSTEM" and value == "1":
                enabled_features.append("- File system.")
            if name == "CONFIG_CONSOLE" and value != "CONFIG_CONSOLE_NONE":
                enabled_features.append("- :doc:`Console.<../library-reference/oam/console>`")
            if name == "CONFIG_SHELL" and value == "1":
                enabled_features.append("- :doc:`Debug shell.<../library-reference/oam/shell>`")

        rst = BOARD_FMT.format(name=board,
                               desc=data["board_desc"],
                               desc_underline="=" * len(data["board_desc"]),
                               homepage=data["board_homepage"],
                               pinout=data["board_pinout"],
                               enabled_features='\n'.join(enabled_features),
                               mcu=data["mcu"].replace("/", ""),
                               drivers='\n'.join(drivers),
                               default_configuration=default_configuration,
                               include_extra=include_extra,
                               default_configuration_targets='\n\n'.join(
                                   default_configuration_targets))

        rst_path = os.path.join("doc", "boards", board + ".rst")
        print "Writing to ", rst_path
        with open(rst_path, "w") as fout:
            fout.write(rst)


def examples_generate(_):
    """Generate examples.

    """

    for example in ["blink", "hello_world", "shell", "http_client"]:
        c_path = os.path.join("examples", example, "main.c")

        source = []
        with open(c_path) as fin:
            for line in fin.readlines():
                source.append("   " + line)

        rst = SOURCE_CODE_FMT.format(source=''.join(source))

        rst_path = os.path.join("doc", "examples", example, "source-code.rst")

        print "Writing to ", rst_path
        with open(rst_path, "w") as fout:
            fout.write(rst)


def main():
    """Main.

    """

    args = get_arguments()

    with open(args.database) as fin:
        database = json.load(fin)

    boards_generate(database)
    examples_generate(database)


if __name__ == "__main__":
    main()
