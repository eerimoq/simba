#!/usr/bin/env python

import serial
import sys
import subprocess
import os
import time
import string

target = sys.argv[1]
exe = sys.argv[2]
baudrate = sys.argv[3]
simba_path = sys.argv[4]
runlog = sys.argv[5]
run_end_pattern = sys.argv[6]
run_end_pattern_success = sys.argv[7]
binary = sys.argv[8]

def run(command):
    print " ".join(command)
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    lines_iterator = iter(proc.stdout.readline, b"")
    with open(runlog, "w") as fout:
        for line in lines_iterator:
            sys.stdout.write(line)
            filtered_line = "".join([char
                                     for char in line
                                     if char in string.printable])
            fout.write(filtered_line)
    res = proc.wait()
    if res != 0:
        sys.exit(res)


def _upload():
    """Try to upload.
    """

    print ("Setting /dev/arduino to 1200 baud and setting DTR to "
           "reset the board.")
    ser = serial.Serial("/dev/arduino", baudrate=1200)
    ser.setDTR(1)
    ser.close()
    time.sleep(0.4)
    command = "bossac --port=" + bossac_port + " -e -w -b -R " + binary
    print command
    subprocess.check_call(command, shell=True)

def upload():
    """Upload.

    """

    try:
        _upload()
    except:
        try:
            # /dev/arduino cannot be opened unless a COM-port is
            # opened first
            subprocess.check_call("bossac --port="
                                  + bossac_port
                                  + " -U false -e -w -b "
                                  + binary,
                                  shell=True)
        except:
            _upload()


if "AVRDUDE_PORT" in os.environ:
    bossac_port = os.environ["AVRDUDE_PORT"]
else:
    bossac_port = "arduino"

if target == "run":
    upload()
    try:
        run(["python",
             "-u",
             os.path.join(simba_path, "make/run.py"),
             run_end_pattern,
             run_end_pattern_success,
             baudrate])
    except:
        sys.exit(1)
elif target == "upload":
    upload()
else:
    print "Bad target ${TARGET}."
    sys.exit(1)
