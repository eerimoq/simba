#!/usr/bin/env python

import serial
import sys
import subprocess
import os
import time

target = sys.argv[1]
exe = sys.argv[2]
simba_path = sys.argv[3]
binary = sys.argv[4]

def upload(bossac_port):
    print "Setting /dev/arduino to 1200 baud and setting DTR to reset the board."
    ser = serial.Serial("/dev/arduino", baudrate=1200)
    ser.setDTR(1)
    ser.close()
    time.sleep(0.4)
    print "bossac --port=" + bossac_port + " -e -w -b " + binary
    subprocess.check_call("bossac --port=" + bossac_port + " -U false -e -w -b " + binary,
                          shell=True)
    subprocess.check_call(os.path.join(simba_path, "make/run.py"), shell=True)

if "AVRDUDE_PORT" in os.environ:
	bossac_port = os.environ["AVRDUDE_PORT"]
else:
	bossac_port = "/dev/arduino"

if target == "run":
    try:
        upload(bossac_port)
    except:
        try:
            # /dev/arduino cannot be opened unless a COM-port is opened first
            subprocess.check_call("bossac --port=" + bossac_port + " -U false -e -w -b " + binary,
                                  shell=True)
        except:
            upload(bossac_port)
else:
    print "Bad target ${TARGET}."
    sys.exit(1)
