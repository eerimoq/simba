#!/usr/bin/env python

"""Release handling.
"""

import os
import subprocess
import argparse
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

    # run linux tests
    clean()

    command = [
        "make",
        "test"
    ]

    print " ".join(command)

    subprocess.check_call(command)


def main():
    """Main.
    """

    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--test",
        action="store_true",
        help="Test that the code about to be release works.")

    args = parser.parse_args()

    # first of all, the repository must be clean
    raw_input("ATTENTION: All files not version controlled by git will be "
              "_removed_. Press any key to continue or ctrl-c to exit.")
    print

    # run the tests before anything else
    if args.test:
        test()


if __name__ == "__main__":
    main()
