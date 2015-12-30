#!/usr/bin/env python

import sys
import subprocess

target = sys.argv[1]
exe = sys.argv[2]
simba_path = sys.argv[3]
runlog = sys.argv[4]

def run(command):
    proc = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE)
    lines_iterator = iter(proc.stdout.readline, b"")
    with open(runlog, "w") as fout:
        for line in lines_iterator:
            sys.stdout.write(line)
            fout.write(line)
    res = proc.wait()
    if res != 0:
        sys.exit(res)


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
