#!/usr/bin/env python3

import re
import json
import os


SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
ERRNOS_JSON = os.path.join(SCRIPT_DIR, "errnos.json")


def human_readable_errno(code_in):
    """Return a human readable string of given error code.

    """

    with open(ERRNOS_JSON) as fin:
        errnos = json.load(fin)

    for _, code, description in errnos:
        if code == code_in:
            return description


def main():
    re_errno = re.compile(r"/\*\* (?P<comment>[^\*]*)\*/"
                          r"\s*#define\s+(?P<name>\w+)\s+(?P<number>\d+)",
                          re.DOTALL)

    errnos = []

    with open("../src/kernel/errno.h") as f:
        for description, name, code in re_errno.findall(f.read()):
            errnos.append((name, int(code), description.strip()[:-1].lower()))

    with open(ERRNOS_JSON, "w") as fout:
        fout.write(json.dumps(errnos, indent=4))


if __name__ == '__main__':
    main()
