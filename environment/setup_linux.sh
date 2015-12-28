#
# Setup script for Linux. Must be sourced from the simba root folder.
#

# Add the simba bin folder to the path
export PATH=$PATH:$(readlink -f bin)
export SIMBA_ROOT=$(readlink -f .)
export BOARD=linux
