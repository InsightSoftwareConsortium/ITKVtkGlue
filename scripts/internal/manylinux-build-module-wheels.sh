#!/usr/bin/env bash

set -x
set -e

# -----------------------------------------------------------------------
# Versions can be restricted by passing them in as arguments to the script
# For example,
# ./scripts/dockcross-manylinux-build-module-wheels.sh cp35
if [[ $# -eq 0 ]]; then
  PYBIN=(/opt/python/*/bin)
  PYBINARIES=()
  for version in "${PYBIN[@]}"; do
    # Limited by VTK Python package supported versions
    if [[  ${version} == *"cp35"* || ${version} == *"cp36"* ]]; then
      PYBINARIES+=(${version})
    fi
  done
else
  PYBINARIES=()
  for version in "$@"; do
    PYBINARIES+=(/opt/python/*${version}*/bin)
  done
fi
ARCH="x64"
PYTHON_LIBRARY=""
PYTHON_LIBRARY=$(cd $(dirname $0); pwd)/libpython-not-needed-symbols-exported-by-interpreter
touch ${PYTHON_LIBRARY}

script_dir=$(cd $(dirname $0) || exit 1; pwd)

for PYBIN in "${PYBINARIES[@]}"; do
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
    echo 'VTK build tree not available!' 1>&2
    exit 1
  fi
  itk_source_dir=/work/standalone-${ARCH}-build/VTK-source
  ln -fs /VTKPythonPackage/standalone-${ARCH}-build/VTK-source /work/standalone-${ARCH}-build/VTK-source
  if [[ ! -d ${itk_source_dir} ]]; then
    echo 'VTK source tree not available!' 1>&2
    exit 1
  fi

  ${PYBIN}/python -m pip install --upgrade scikit-build

  ${PYBIN}/python setup.py bdist_wheel --build-type MinSizeRel -- \
    -Wno-dev \
    -DITK_DIR:PATH=${itk_build_dir} \
    -DVTK_DIR:PATH=${vtk_build_dir} \
    -DCMAKE_CXX_COMPILER_TARGET:STRING=$(uname -p)-linux-gnu \
    -DBUILD_TESTING:BOOL=OFF \
    -DITK_WRAP_PYTHON:BOOL=ON \
    -DITK_USE_SYSTEM_SWIG:BOOL=ON \
    -DSWIG_EXECUTABLE:FILEPATH=${itk_build_dir}/Wrapping/Generators/SwigInterface/swig/bin/swig \
    -DPYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE} \
    -DPYTHON_INCLUDE_DIR:PATH=${PYTHON_INCLUDE_DIR} \
    -DPYTHON_LIBRARY:FILEPATH=${PYTHON_LIBRARY} \
    || exit 1
  ${PYBIN}/python setup.py clean

  #cd /work/build
  #rm -rf *
  #cmake \
    #-Wno-dev \
    #-G Ninja \
    #-DCMAKE_BUILD_TYPE:STRING=MinSizeRel \
    #-DITK_DIR:PATH=${itk_build_dir} \
    #-DVTK_DIR:PATH=${vtk_build_dir} \
    #-DCMAKE_CXX_COMPILER_TARGET:STRING=$(uname -p)-linux-gnu \
    #-DBUILD_TESTING:BOOL=ON \
    #-DITK_WRAP_PYTHON:BOOL=ON \
    #-DITK_USE_SYSTEM_SWIG:BOOL=ON \
    #-DSWIG_EXECUTABLE:FILEPATH=${itk_build_dir}/Wrapping/Generators/SwigInterface/swig/bin/swig \
    #-DPYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE} \
    #-DPYTHON_INCLUDE_DIR:PATH=${PYTHON_INCLUDE_DIR} \
    #-DPYTHON_LIBRARY:FILEPATH=${PYTHON_LIBRARY} \
    #..
  #ninja
done

# This step will fixup the wheel switching from 'linux' to 'manylinux1' tag
for whl in dist/*linux_$(uname -p).whl; do
    auditwheel repair ${whl} -w /work/dist/
    rm ${whl}
done
