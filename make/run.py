#!/usr/bin/env python

import sys
import serial
import expect

dev_serial = serial.Serial("/dev/arduino", baudrate=38400, timeout=10)
dev = expect.Handler(dev_serial)

status = 0

try:
    report = dev.expect(r"harness report: total\(\d+\), passed\(\d+\), failed\(\d+\)", timeout=10)
except:
    status = 1
else:
    if "failed(0)" not in report:
        status = 1

sys.exit(status)