#!/usr/bin/env python

import sys
import subprocess
import os

target = sys.argv[1]
exe = sys.argv[2]
simba_path = sys.argv[3]


def run(command):
    print command
    try:
        subprocess.check_call(command, shell=True)
    except:
        sys.exit(1)
    

if target == "debugger":
    run("gdb {exe} --eval-command \"break main\" --eval-command run".format(exe=exe))
elif target == "profile":
    run("gprof " + exe)
elif target == "coverage":
    run("geninfo . -o coverage.info")
    run("genhtml coverage.info")
elif target == "jenkins-coverage":
    run("gcovr -r $(readlink -f ../../..) -x -e \".*main.c\"")
elif target == "run":
    run(exe)
else:
    print "Bad target " + target
    sys.exit(1)
