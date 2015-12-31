#!/usr/bin/env python

import sys
import serial
import expect
import re

pattern = sys.argv[1]
pattern_success = sys.argv[2]

dev_serial = serial.Serial("/dev/arduino", baudrate=38400, timeout=10)
dev = expect.Handler(dev_serial)

status = 0

try:
    report = dev.expect(pattern, timeout=10)
except:
    status = 1

if not re.match(pattern_success, report):
    status = 1

print

sys.exit(status)
