#
# Setup script for Linux. Must be sourced from the simba root folder.
#

# Add the simba bin folder to the path
export PATH=$PATH:$(readlink -f bin)

# ESP8266 toolchain
export PATH=$PATH:$(readlink -f ../esp-open-sdk/xtensa-lx106-elf/bin)

export SIMBA_ROOT=$(readlink -f .)
export BOARD=linux

export PYLINTRC=$(readlink -f environment/pylintrc)
