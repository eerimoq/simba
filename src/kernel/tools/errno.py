#!/usr/bin/env python

import sys
import re
from collections import OrderedDict
import json

re_errno = re.compile(r"#define\s+(?P<name>\w+)\s+(?P<number>\d+)\s+/\*(?P<comment>.*)\*/")

errno_map = OrderedDict()

with open("../kernel/errno.h") as f:
    for line in f:
        mo = re_errno.match(line)
        if mo:
            errno_map[int(mo.group("number"))] = {"name": mo.group("name"),
                                                  "comment": mo.group("comment").strip()}

sys.stdout.write("errno_map = ")
sys.stdout.write(json.dumps(errno_map, indent=4))
