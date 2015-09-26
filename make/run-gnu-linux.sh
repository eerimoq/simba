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

if [ ${TARGET} == debugger ] ; then
    gdb ${EXE} --eval-command "break main" --eval-command run
elif [ ${TARGET} == profile ] ; then
    gprof ${EXE}
elif [ ${TARGET} == coverage ] ; then
    geninfo . -o coverage.info
    genhtml coverage.info
elif [ ${TARGET} == jenkins-coverage ] ; then
    gcovr -r $(readlink -f ../../..) -x -e ".*main.c"
elif [ ${TARGET} == run ] ; then
    ${EXE}
else
    echo "Bad target ${TARGET}."
    exit 1
fi
