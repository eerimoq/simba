#
# Setup script for Cygwin on Windows.
#

# AVR toolchain
export PATH=$PATH:$(readlink -f ../avr-toolchain-windows/bin)

# ARM toolchain
export PATH=$PATH:$(readlink -f ../arm-toolchain-windows/arm-none-eabi-gcc/4.8.3-2014q1/bin)

mkdir -p .cygwin

# Serial port
read -p "Enter Arduino COM port (for example COM3): " com_port

export AVRDUDE_PORT=${com_port}

(cd /dev && ln -sf $(python -c "import sys; sys.stdout.write('ttyS' + str(int(sys.argv[1][3:]) - 1))" $com_port) arduino)