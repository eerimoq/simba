#
# Setup script for Linux. Must be sourced from the simba root folder.
#

# Add the simba bin folder to the path
export PATH=$PATH:$(readlink -f bin)

# ESP8266 toolchain
export PATH=$PATH:$(readlink -f ../esp-open-sdk/xtensa-lx106-elf/bin)

# RUST bindgen
export PATH=$PATH:$(readlink -f ../rust-bindgen/target/debug)

# st-link
export PATH=$PATH:$(readlink -f ../stlink)

export SIMBA_ROOT=$(readlink -f .)
export BOARD=linux

export PYLINTRC=$(readlink -f environment/pylintrc)

# Arduino release repositories.
export SIMBA_ARDUINO_AVR_ROOT=$(readlink -f ../simba-arduino-avr)
export SIMBA_ARDUINO_SAM_ROOT=$(readlink -f ../simba-arduino-sam)
