#!/usr/bin/env bash

set -e

FILENAME=sdcard

rm -f ${FILENAME}
seq 1000 | xargs > ${FILENAME}
