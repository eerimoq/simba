#!/usr/bin/env python

"""Release handling.
"""

import os
import subprocess
import glob


def clean():
    """Clean.
    """

    subprocess.check_call(["git", "clean", "-dfx"])


def test():
    """Test.

    """

    all_boards = glob.glob("src/boards/*")
    boards = set(all_boards) - set("cygwin")

    for board in boards:
        clean()

        # Building one application is enough to ensure that all code
        # in src/ compiles.
        command = [
            "make",
            "APPS=tst/kernel/sys",
            "BOARD=" + os.path.basename(board),
            "all"
        ]

        print " ".join(command)

        subprocess.check_call(command)

    # Run linux tests.
    clean()

    command = [
        "make",
        "test"
    ]

    print " ".join(command)

    subprocess.check_call(command)

    generate_platformio()

    # Build a PlatformIO application.
    command = [
        "platformio",
        "run"
    ]

    subprocess.check_call(command, cwd="examples/platformio/blink")

    generate_arduino()

    # Build an application using the Arduino builder.
    for family, board in [("avr", "nano"),
                          ("avr", "uno"),
                          ("avr", "mega2560"),
                          ("avr", "pro-micro"),
                          ("sam", "arduino_due_x_dbg"),
                          ("esp", "esp01"),
                          ("esp", "esp12e")]:
        command = [
            "make",
            "all",
            "FAMILY=" + family,
            "BOARD=" + board
        ]

        subprocess.check_call(command, cwd="examples/arduino/blink")


def generate_docs():
    """Generate the documentation.

    """

    command = [
        "make",
        "doc"
    ]

    subprocess.check_call(command)


def build_examples():
    """Build the examples.

    """

    clean()

    command = [
        "make"
    ]

    subprocess.check_call(command, cwd="examples")


def generate_platformio():
    """Generate the platformio files.

    """

    command = [
        "make",
        "platformio"
    ]

    subprocess.check_call(command)


def generate_arduino():
    """Generate the arduino files.

    """

    command = [
        "make",
        "arduino"
    ]

    subprocess.check_call(command)


def main():
    """Main function.

    """

    # first of all, the repository must be clean
    raw_input("ATTENTION: All files not version controlled by git will be "
              "_removed_. Press any key to continue or ctrl-c to exit.")
    print

    test()
    build_examples()

    # Generate the documenation after the last "git clean -dfx"
    # execution.
    generate_docs()


if __name__ == "__main__":
    main()
