#
# Setup script for Linux.
#

# Add the simba bin folder to the path
export PATH=$PATH:$(readlink -f bin)
export SIMBA_ROOT=$(readlink -f .)
