#!/bin/bash

# Run this script to build and test the ITKVTKGlue external module.
#
# Extra ctest arguments can be passed in as arguments to the script
# For example,
#
#   scripts/dockcross-manylinux-build-module-wheels.sh -V

script_dir=$(cd $(dirname $0) || exit 1; pwd)

docker build -t insighttoolkit/itkvtkglue $script_dir

# Generate dockcross scripts
docker run insighttoolkit/itkvtkglue > /tmp/insighttoolkit-itkvtkglue
chmod u+x /tmp/insighttoolkit-itkvtkglue


mkdir -p build
DOCKER_ARGS="-v $(pwd)/build:/work/build/ -v $script_dir/../ITKPythonPackage:/ITKPythonPackage -v $script_dir/../VTKPythonPackage:/VTKPythonPackage"
/tmp/insighttoolkit-itkvtkglue \
  -a "$DOCKER_ARGS" \
  "/work/scripts/internal/dockcross-manylinux-build-test.sh" "$@"
