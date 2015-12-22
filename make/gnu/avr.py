#!/usr/bin/env python

import sys
import subprocess
import os
import re

target = sys.argv[1]
exe = sys.argv[2]
simba_path = sys.argv[3]
run_end_pattern = sys.argv[4]
run_end_pattern_success = sys.argv[5]
mcu = sys.argv[6]
hex_file = sys.argv[7]
settings_bin = sys.argv[8]
rest = sys.argv[9:]


def run(command):
    print ' '.join(command)
    try:
        subprocess.check_call(command)
    except:
        sys.exit(1)

if target == "run":
    run(["avrdude", "-p", mcu] + rest + ["-U", "eeprom:w:" + settings_bin + ":r"])
    run(["avrdude", "-p", mcu] + rest + ["-U", "flash:w:" + hex_file])
    run([os.path.join(simba_path, "make/run.py"),
         run_end_pattern,
         run_end_pattern_success])
elif TARGET == "dump":
    run(["avrdude", "-p", mcu] + rest + ["-U", "eeprom:r:eeprom.bin:r"])
    run([os.path.join(simba_path, "make/dumpdecoder.py"), "eeprom.bin"])
else:
    print "Bad target " + target
    sys.exit(1)
