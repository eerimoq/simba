#!/usr/bin/env python

import sys
import struct

errno_to_string = {
}

errno, mcusr = struct.unpack('ii', open(sys.argv[1]).read()[0:8])

print('0x0000:errno = {message} ({errno})'.format(message=message, errno=errno))
print('0x0004:mcusr = {message} ({mcusr})'.format(message=message, mcusr=mcusr))
