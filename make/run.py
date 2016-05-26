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
print "INFO: RUN_END_PATTERN = '{}'".format(pattern)
print "INFO: RUN_END_PATTERN_SUCCESS = '{}'".format(pattern_success)

try:
    print "INFO: Waiting for RUN_END_PATTERN."
    print
    print "INFO: Application output begin."
    print
    report = dev.expect(pattern, timeout=10)
    print
    print
    print "INFO: Application output end."
    print
except:
    print
    print
    print "INFO: Application output end."
    print
    print "WARNING: RUN_END_PATTERN was never found."
    print
    status = 1
else:
    if not re.match(pattern_success, report):
        print
        print "ERROR: End pattern string does not match RUN_END_PATTERN_SUCCESS."
        print
        status = 1

print

sys.exit(status)
