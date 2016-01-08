#!/usr/bin/env python

"""Generate documentation.
"""

import os
import argparse
import json


ABOUT_FMT = """{boards}
"""

BOARD_FMT = """{desc}
===========

Homepage
--------

{homepage}

Pinout
------

.. image:: ../images/boards/{pinout}
   :width: 50%
   :target: ../_images/{pinout}

Drivers
-------

{drivers}

Mcu
---

:doc:`../mcus/{mcu}`

API Reference
-------------

:doc:`{desc} <../api-reference/boards/{name}>`

.. include:: extra/{name}.rst
"""

MCU_FMT = """{desc}
==========

Homepage
---------

{homepage}

.. include:: extra/{name}.rst
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


def about_generate(database):
    """Generate about.

    """

    boards = []
    for board in sorted(database["boards"]):
        boards.append("- :doc:`../boards/{}`".format(board))

    rst = ABOUT_FMT.format(boards='\n'.join(boards))

    rst_path = os.path.join("doc", "about", "about.rst")
    print "Writing to ", rst_path
    with open(rst_path, "w") as fout:
        fout.write(rst)


def boards_generate(database):
    """Generate boards.

    """

    for board, data in database["boards"].items():
        drivers = []
        for driver in sorted(data["drivers"]):
            drivers.append("- :doc:`../api-reference/drivers/{}`".format(
                driver))
        rst = BOARD_FMT.format(name=board,
                               desc=data["board_desc"],
                               homepage=data["board_homepage"],
                               pinout=data["board_pinout"],
                               mcu=data["mcu"].replace("/", ""),
                               drivers='\n'.join(drivers))

        rst_path = os.path.join("doc", "boards", board + ".rst")
        print "Writing to ", rst_path
        with open(rst_path, "w") as fout:
            fout.write(rst)


def mcus_generate(database):
    """Generate MCU.

    """

    for mcu, data in database["mcus"].items():
        rst = MCU_FMT.format(name=mcu,
                             desc=data["mcu_name"],
                             homepage=data["mcu_homepage"])

        rst_path = os.path.join("doc", "mcus", mcu + ".rst")
        print "Writing to ", rst_path
        with open(rst_path, "w") as fout:
            fout.write(rst)


def examples_generate(_):
    """Generate examples.

    """

    for example in ["blink", "hello_world", "shell"]:
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

    about_generate(database)
    boards_generate(database)
    mcus_generate(database)
    examples_generate(database)


if __name__ == "__main__":
    main()
