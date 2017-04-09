#
# Setup script for the Simba development environment. Source this file
# in your shell to setup the environment.
#

if [ "${OSTYPE}" = "linux-gnu" ]; then
    source environment/setup_linux.sh
else
    echo "\$\{OSTYPE\} must be linux-gnu."
    exit 1
fi
