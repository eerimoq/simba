#!/usr/bin/env python

import sys
import serial
import expect
import re

pattern = sys.argv[1]
pattern_success = sys.argv[2]
baudrate = sys.argv[3]

dev_serial = serial.Serial("/dev/arduino",
                           baudrate=int(baudrate),
                           timeout=10)
dev = expect.Handler(dev_serial)

status = 0

print
print "INFO: Pattern:", pattern
print "INFO: Success pattern:", pattern_success

try:
    print "INFO: Waiting for the end pattern."
    print
    report = dev.expect(pattern, timeout=10)
except:
    print
    print "WARNING: The end pattern was never received."
    print
    status = 1
else:
    if not re.match(pattern_success, report):
        print
        print "ERROR: The end pattern does not match the success end pattern."
        print
        status = 1

print

sys.exit(status)
