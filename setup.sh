#
# Setup script for Linux
#

# Add the simba bin folder to the path
export PATH=$PATH:$(readlink -f bin)
export SIMBA_ROOT=$(readlink -f .)

dist_packages=$(ls dist-packages | grep -v dist-info | grep -v README.rst)

for name in ${dist_packages} ; do
    root=$(python -c "import sys; sys.stdout.write('${name}'.upper() + '_ROOT')")
    export ${root}=$(readlink -f dist-packages/${name})
done
