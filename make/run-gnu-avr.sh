#!/usr/bin/env bash
#
# @file make/run-gnu.sh
# @version 1.0
#
# @section License
# Copyright (C) 2014-2015, Erik Moqvist
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# This file is part of the Simba project.
#

TARGET=$1
EXE=$2
SIMBA=$3
MCU=$4
HEX=$5
shift 5

if [ ${TARGET} == run ] ; then
    if avr-size --mcu=${MCU} --format=avr ${EXE} | python -c "import re ; import sys ; sys.exit(len([ v for v in [re.search('\((\d+)', s).group(1) for s in sys.stdin.readlines() if re.search('\((\d+)', s)] if int(v) >= 100 ]))" ; then
        echo "avrdude -p ${MCU} $@ -U flash:w:${HEX}"
        avrdude -p ${MCU} $@ -U flash:w:${HEX} 2>&1
        ${SIMBA}/make/run.exp
        exit $?
    else
        echo "Program does not fit in memory. Aborting."
    fi
elif [ ${TARGET} == dump ] ; then
        avrdude -p ${MCU} $@ -U eeprom:r:eeprom.bin:r 2>&1
        ${SIMBA}/make/sys.py eeprom.bin
else
    echo "Bad target ${TARGET}."
    exit 1
fi
