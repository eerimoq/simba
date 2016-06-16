#
# Setup script for Cygwin on Windows. Must be sourced from the simba
# root folder.
#

# Add the simba bin folder to the path
export PATH=$PATH:$(readlink -f bin)

# ESP8266 toolchain
export PATH=$PATH:$(readlink -f ../esp-open-sdk/xtensa-lx106-elf/bin)

export SIMBA_ROOT=$(readlink -f .)
export BOARD=cygwin

# RUST bindgen
export PATH=$PATH:$(readlink -f ../rust-bindgen/target/debug)

# AVR toolchain
export PATH=$PATH:$(readlink -f ../avr-toolchain-windows/bin)

# ARM toolchain
export PATH=$PATH:$(readlink -f ../arm-toolchain-windows/arm-none-eabi-gcc/4.8.3-2014q1/bin)
export PATH=$PATH:$(readlink -f ../arm-toolchain-windows/bossac/1.3a-arduino)

# Serial port
read -p "Enter Arduino COM port (for example COM3) or just hit Enter to skip this step: " com_port

if [ ! -z "$com_port" ] ; then
    export AVRDUDE_PORT=${com_port}

	(cd /dev && ln -sf $(python -c "import sys; sys.stdout.write('ttyS' + str(int(sys.argv[1][3:]) - 1))" $com_port) arduino)
fi

export PYLINTRC=$(readlink -f environment/pylintrc)

export PYTHONPATH=$(readlink -f 3pp/python-readchar-1.1.0)

# Arduino release repository.
export SIMBA_ARDUINO_ROOT=$(readlink -f ../simba-arduino)
