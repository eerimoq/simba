#!/usr/bin/env python

import sys
import struct

errno = struct.unpack('i', open(sys.argv[1]).read()[0:4])[0]

print('''0x0000:errno = {errno}'''.format(errno = errno))
