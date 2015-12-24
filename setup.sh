#
# Setup script for the Simba development environment. Source this file
# in your shell to setup the environment.
#

if [ "${OSTYPE}" == "linux-gnu" ]; then
    source setup_linux.sh
elif [ "${OSTYPE}" == "cygwin" ]; then
    source setup_cygwin.sh
else
    echo "\$\{OSTYPE\} must be linux-gnu or cygwin."
    exit 1
fi
