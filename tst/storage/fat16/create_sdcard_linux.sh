#!/usr/bin/env bash
# create a empty FAT16 file system in a file called 'sdcard'

set -e

FILENAME=sdcard

rm -f ${FILENAME}
dd bs=1M count=16 if=/dev/zero of=${FILENAME}

# The test suite will format the disk.

# parted ${FILENAME} mklabel msdos
# mkdosfs -F 16 ${FILENAME}
# parted ${FILENAME} print
