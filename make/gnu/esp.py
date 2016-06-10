#!/usr/bin/env python

import sys
import subprocess
import os
import re

target = sys.argv[1]
exe = sys.argv[2]
baudrate = sys.argv[3]
simba_path = sys.argv[4]
runlog = sys.argv[5]
run_end_pattern = sys.argv[6]
run_end_pattern_success = sys.argv[7]
binary = sys.argv[8]


def run(command):
    """Run a command.

    """
    
    print " ".join(command)
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    lines_iterator = iter(proc.stdout.readline, b"")
    with open(runlog, "w") as fout:
        for line in lines_iterator:
            sys.stdout.write(line)
            fout.write(line)
    res = proc.wait()
    if res != 0:
        sys.exit(res)


def upload():
    """Upload.
    """

    command = [
        os.path.join(simba_path, "3pp", "esptool", "esptool.py"),
        "--baud",
        "460800",
        "write_flash",
        "0x00000",
        os.path.join(simba_path,
                     "3pp",
                     "ESP8266_RTOS_SDK",
                     "bin",
                     "boot_v1.4.bin"),
        "0x01000",
        binary
    ]
    print " ".join(command)
    subprocess.check_call(command)


if target == "run":
    run(["python",
         "-u",
         os.path.join(simba_path, "make", "run.py"),
         run_end_pattern,
         run_end_pattern_success,
         "38400"])
elif target == "upload":
    upload()
else:
    print "Bad target " + target
    sys.exit(1)
