#!/usr/bin/env bash

set -x
set -e

ARCH="x64"
PYTHON_LIBRARY=""
PYBIN="/opt/python/cp36-cp36m/bin"

script_dir=$(cd $(dirname $0) || exit 1; pwd)

PYTHON_EXECUTABLE=${PYBIN}/python
PYTHON_INCLUDE_DIR=$( find -L ${PYBIN}/../include/ -name Python.h -exec dirname {} \; )

echo ""
echo "PYTHON_EXECUTABLE:${PYTHON_EXECUTABLE}"
echo "PYTHON_INCLUDE_DIR:${PYTHON_INCLUDE_DIR}"
echo "PYTHON_LIBRARY:${PYTHON_LIBRARY}"

itk_build_dir=/work/ITK-$(basename $(dirname ${PYBIN}))-manylinux1_${ARCH}
ln -fs /ITKPythonPackage/ITK-$(basename $(dirname ${PYBIN}))-manylinux1_${ARCH} $itk_build_dir
if [[ ! -d ${itk_build_dir} ]]; then
  echo 'ITK build tree not available!' 1>&2
  exit 1
fi
mkdir -p /work/standalone-${ARCH}-build
itk_source_dir=/work/standalone-${ARCH}-build/ITK-source
ln -fs /ITKPythonPackage/standalone-${ARCH}-build/ITK-source /work/standalone-${ARCH}-build/ITK-source
if [[ ! -d ${itk_source_dir} ]]; then
  echo 'ITK source tree not available!' 1>&2
  exit 1

fi
vtk_build_dir=/work/VTK-$(basename $(dirname ${PYBIN}))-manylinux1_${ARCH}
ln -fs /VTKPythonPackage/VTK-$(basename $(dirname ${PYBIN}))-manylinux1_${ARCH} $vtk_build_dir
if [[ ! -d ${vtk_build_dir} ]]; then
  echo 'VTK build tree not available!' 1>&
  exit 1
fi

cd /work/build
rm -rf *
cmake \
  -Wno-dev \
  -G Ninja \
  -DCMAKE_BUILD_TYPE:STRING=MinSizeRel \
  -DITK_DIR:PATH=${itk_build_dir} \
  -DVTK_DIR:PATH=${vtk_build_dir} \
  -DCMAKE_CXX_COMPILER_TARGET:STRING=$(uname -p)-linux-gnu \
  -DBUILD_TESTING:BOOL=ON \
  -DITK_WRAP_PYTHON:BOOL=ON \
  -DITK_USE_SYSTEM_SWIG:BOOL=ON \
  -DSWIG_EXECUTABLE:FILEPATH=${itk_build_dir}/Wrapping/Generators/SwigInterface/swig/bin/swig \
  -DPYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE} \
  -DPYTHON_INCLUDE_DIR:PATH=${PYTHON_INCLUDE_DIR} \
  -DPYTHON_LIBRARY:FILEPATH=${PYTHON_LIBRARY} \
  ..
ninja
LD_LIBRARY_PATH=$vtk_build_dir/lib ctest "$@"
