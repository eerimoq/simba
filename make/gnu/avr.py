#!/usr/bin/env python

import sys
import subprocess
import os
import re

target = sys.argv[1]
exe = sys.argv[2]
simba_path = sys.argv[3]
mcu = sys.argv[4]
hex_file = sys.argv[5]
settings_bin = sys.argv[6]
rest = ' '.join(sys.argv[7:])


def run(command):
    print command
    subprocess.check_call(command, shell=True)
    

if target == "run":
    run("avrdude -p {mcu} {rest} -U eeprom:w:{settings_bin}:r".format(mcu=mcu,
                                                                      rest=rest,
                                                                      settings_bin=settings_bin))
    run("avrdude -p {mcu} {rest} -U flash:w:{hex_file} 2>&1".format(mcu=mcu,
                                                                    rest=rest,
                                                                    hex_file=hex_file))
    run(os.path.join(simba_path, "make/run.py"))
elif TARGET == "dump":
    run("avrdude -p {mcu} {rest} -U eeprom:r:eeprom.bin:r 2>&1".format(mcu=mcu,
                                                                       rest=rest))
    run(os.path.join(simba_path, "/make/dumpdecoder.py eeprom.bin"))
else:
    print "Bad target " + target
    sys.exit(1)
