#!/usr/bin/env python

import sys
import struct
from simbaerrno import errno_map

errno, mcusr = struct.unpack('ii', open(sys.argv[1]).read()[0:8])

if errno == -1:
    errno_desc = {"name": "RUNNING", "comment": "System running."}
elif errno == 0:
    errno_desc = {"name": "OK", "comment": "Normal stop."}
else:
    errno = str(errno)
    errno_desc = errno_map.get(errno, {"name": "OK", "comment": "Normal stop."})
print('0x0000:errno = {errno} ({name}) {comment}'.format(errno=errno,
                                                         name=errno_desc["name"],
                                                         comment=errno_desc["comment"]))

message = []
if mcusr & 0x8:
    message.append("WDRF")
if mcusr & 0x4:
    message.append("BORF")
if mcusr & 0x2:
    message.append("EXTRF")
if mcusr & 0x1:
    message.append("PORF")

print('0x0004:mcusr = {mcusr} ({message})'.format(mcusr=mcusr,
                                                  message=' | '.join(message)))
