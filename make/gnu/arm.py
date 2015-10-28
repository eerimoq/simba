#!/usr/bin/env python

import serial
import sys
import subprocess
import os

target = sys.argv[1]
exe = sys.argv[2]
simba_path = sys.argv[3]
binary = sys.argv[4]

if target == "run":
    print "Setting /dev/arduino to 1200 baud."
    ser = serial.Serial("/dev/arduino", baudrate=1200)
    ser.close()
    print "bossac --port=arduino -e -w -b " + binary
    subprocess.check_call("bossac --port=arduino -e -w -b " + binary,
                          shell=True)
    subprocess.check_call(os.path.join(simba_path, "make/run.exp"), shell=True)
else:
    print "Bad target ${TARGET}."
    sys.exit(1)
