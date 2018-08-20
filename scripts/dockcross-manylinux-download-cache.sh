#!/bin/bash

# This module should be pulled and run from an ITKModule root directory to generate the Linux python wheels of this module,
# it is used by the circle.yml file contained in ITKModuleTemplate: https://github.com/InsightSoftwareConsortium/ITKModuleTemplate

wget -L https://data.kitware.com/api/v1/file/592dd8068d777f16d01e1a92/download -O zstd-1.2.0-linux.tar.gz
gunzip -d zstd-1.2.0-linux.tar.gz
tar xf zstd-1.2.0-linux.tar

curl -L https://github.com/InsightSoftwareConsortium/ITKPythonBuilds/releases/download/${ITK_PACKAGE_VERSION:=v5.0a02}/ITKPythonBuilds-linux.tar.zst -O
./zstd-1.2.0-linux/bin/unzstd ITKPythonBuilds-linux.tar.zst -o ITKPythonBuilds-linux.tar
tar xf ITKPythonBuilds-linux.tar

mkdir tools
wget -L http://ftp.stack.nl/pub/users/dimitri/doxygen-1.8.11.linux.bin.tar.gz -O doxygen-1.8.11.linux.bin.tar.gz
tar -xvzf doxygen-1.8.11.linux.bin.tar.gz -C tools

curl -L https://github.com/KitwareMedical/VTKPythonBuilds/releases/download/${VTK_PACKAGE_VERSION:=v8.1.1}/VTKPythonBuilds-linux.tar.zst -O
./zstd-1.2.0-linux/bin/unzstd VTKPythonBuilds-linux.tar.zst -o VTKPythonBuilds-linux.tar
tar xf VTKPythonBuilds-linux.tar
