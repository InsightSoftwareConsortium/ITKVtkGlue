cmake_minimum_required(VERSION 2.8.12)
project(ITKVtkGlue)

if(NOT ITK_SOURCE_DIR)
  include(itk-module-init.cmake)
  find_package(ITK REQUIRED)
endif()


set(ITKVtkGlue_LIBRARIES ITKVtkGlue)

# Skip the header tests because of weak linking
set(ITKVtkGlue_THIRD_PARTY 1)

#
# Add the third party includes and libraries
#

# Add compiler flags needed to use VTK. VTK has been found at this point.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VTK_REQUIRED_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VTK_REQUIRED_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${VTK_REQUIRED_EXE_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${VTK_REQUIRED_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${VTK_REQUIRED_MODULE_LINKER_FLAGS}")

# Older versions of VTK (VTK 5.5 for example) do not have VTK_VERSION, in this
# case it needs to be defined manually
if(NOT VTK_VERSION)
  set(VTK_VERSION "${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.${VTK_BUILD_VERSION}")
endif()
if(NOT VTK_RENDERING_BACKEND)
  set(VTK_RENDERING_BACKEND OpenGL)
endif()
set(_target_freetypeopengl)
if(TARGET vtkRenderingFreeType${VTK_RENDERING_BACKEND})
  set(_target_freetypeopengl vtkRenderingFreeType${VTK_RENDERING_BACKEND})
endif()
set(_rendering_modules )
if(NOT VTK_RENDERING_BACKEND STREQUAL "None")
  set(_rendering_modules
    vtkRendering${VTK_RENDERING_BACKEND}
    vtkInteractionStyle
    vtkInteractionWidgets
    )
endif()

if (${VTK_VERSION} VERSION_LESS ${VERSION_MIN})
  message(ERROR " VtkGlue requires VTK version ${VERSION_MIN} or newer but the current version is ${VTK_VERSION}")
elseif( ${VTK_VERSION} VERSION_LESS 6.0.0 )
  set(ITKVtkGlue_VTK_INCLUDE_DIRS ${VTK_INCLUDE_DIRS})
  set(ITKVtkGlue_VTK_LIBRARIES ${VTK_LIBRARIES})
else()
  set(_wrap_module)
  if(ITK_WRAP_PYTHON)
    set(_wrap_module vtkWrappingPythonCore)
  endif()
  vtk_module_config(ITKVtkGlue_VTK
    vtkRenderingFreeType
    ${_target_freetypeopengl}
    ${_rendering_modules}
    vtkIOImage
    vtkIOLegacy
    vtkImagingSources
    ${_wrap_module}
    )
endif()
# Artifacts of the VTKPythonPackage build configuration
list(FILTER ITKVtkGlue_VTK_LIBRARIES EXCLUDE REGEX
  "libpython-not-needed-symbols")
list(FILTER ITKVtkGlue_VTK_LIBRARIES EXCLUDE REGEX
  "optimized")
list(FILTER ITKVtkGlue_VTK_LIBRARIES EXCLUDE REGEX
  "debug")

# The VTK DICOMParser and vtkmetaio includes conflict with the ITK
# versions. Here we remove them from the include directories.
#
string(REGEX REPLACE "[^;]*MetaIO;"
         "" ITKVtkGlue_VTK_INCLUDE_DIRS "${ITKVtkGlue_VTK_INCLUDE_DIRS}")
string(REGEX REPLACE "[^;]*vtkmetaio;"
         "" ITKVtkGlue_VTK_INCLUDE_DIRS "${ITKVtkGlue_VTK_INCLUDE_DIRS}")
string(REGEX REPLACE "[^;]*DICOMParser;"
         "" ITKVtkGlue_VTK_INCLUDE_DIRS "${ITKVtkGlue_VTK_INCLUDE_DIRS}")
if(ITK_WRAP_PYTHON)
  # for vtkPythonUtil.h, vtkPython.h, vtkPythonConfigure.h,
  # vtkPythonCompatibility.h, vtkPythonConfigure.h, ...
  list(APPEND ITKVtkGlue_VTK_INCLUDE_DIRS
    "${VTK_CMAKE_DIR}/../Wrapping/PythonCore"
    "${VTK_CMAKE_DIR}/../Utilities/Python"
    "${VTK_DIR}/Utilities/Python"
    )
endif()

# Set up the include and library dirs needed for the module
set(ITKVtkGlue_SYSTEM_INCLUDE_DIRS ${ITKVtkGlue_VTK_INCLUDE_DIRS})
set(ITKVtkGlue_SYSTEM_LIBRARY_DIRS ${ITKVtkGlue_VTK_LIBRARY_DIRS})

# Add preprocessor definitions needed to use VTK.
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ${ITKVtkGlue_VTK_DEFINITIONS})

set(ITKVtkGlue_EXPORT_CODE_INSTALL "
set(VTK_DIR \"${VTK_DIR}\")

find_package(VTK NO_MODULE REQUIRED)

if(NOT VTK_VERSION)
  set(VTK_VERSION \"${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.${VTK_BUILD_VERSION}\")
endif()
if(NOT VTK_RENDERING_BACKEND)
  set(VTK_RENDERING_BACKEND OpenGL)
endif()
set(_target_freetypeopengl)
if(TARGET vtkRenderingFreeType${VTK_RENDERING_BACKEND})
  set(_target_freetypeopengl vtkRenderingFreeType${VTK_RENDERING_BACKEND})
endif()
if(NOT VTK_RENDERING_BACKEND STREQUAL \"None\")
  set(_rendering_modules
    vtkRendering${VTK_RENDERING_BACKEND}
    vtkInteractionStyle
    vtkInteractionWidgets
    )
endif()

if( ${VTK_VERSION} VERSION_LESS 6.0.0 )
  set(ITKVtkGlue_VTK_INCLUDE_DIRS ${VTK_INCLUDE_DIRS})
  set(ITKVtkGlue_VTK_LIBRARIES ${VTK_LIBRARIES})
else()
  set(_wrap_module)
  if(ITK_WRAP_PYTHON AND PYTHON_VERSION_STRING VERSION_LESS 3.0)
    set(_wrap_module vtkWrappingPythonCore)
  endif()
  vtk_module_config(ITKVtkGlue_VTK
    vtkRenderingFreeType
    ${_target_freetypeopengl}
    ${_rendering_modules}
    vtkIOImage
    vtkIOLegacy
    vtkImagingSources
    ${_wrap_module}
    )

  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS \${ITKVtkGlue_VTK_DEFINITIONS})

  # Artifacts of the VTKPythonPackage build configuration
  list(FILTER ITKVtkGlue_VTK_LIBRARIES EXCLUDE REGEX
    \"libpython-not-needed-symbols\")
  list(FILTER ITKVtkGlue_VTK_LIBRARIES EXCLUDE REGEX
    \"libpython-not-needed-symbols\")
  list(FILTER ITKVtkGlue_VTK_LIBRARIES EXCLUDE REGEX
    \"optimized\")
  list(FILTER ITKVtkGlue_VTK_LIBRARIES EXCLUDE REGEX
    \"debug\")
endif()
")
set(ITKVtkGlue_EXPORT_CODE_BUILD "
if(NOT ITK_BINARY_DIR)
  set(VTK_DIR \"${VTK_DIR}\")

  find_package(VTK NO_MODULE REQUIRED)

  if(NOT VTK_VERSION)
    set(VTK_VERSION \"${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.${VTK_BUILD_VERSION}\")
  endif()
  if(NOT VTK_RENDERING_BACKEND)
    set(VTK_RENDERING_BACKEND OpenGL)
  endif()
  if(TARGET vtkRenderingFreeType${VTK_RENDERING_BACKEND})
    set(_target_freetypeopengl vtkRenderingFreeType${VTK_RENDERING_BACKEND})
  endif()
  if(NOT VTK_RENDERING_BACKEND STREQUAL \"None\")
    set(_rendering_modules
      vtkRendering${VTK_RENDERING_BACKEND}
      vtkInteractionStyle
      vtkInteractionWidgets
      )
  endif()

  if( ${VTK_VERSION} VERSION_LESS 6.0.0 )
    set(ITKVtkGlue_VTK_INCLUDE_DIRS ${VTK_INCLUDE_DIRS})
    set(ITKVtkGlue_VTK_LIBRARIES ${VTK_LIBRARIES})
  else()
    set(_wrap_module)
    if(ITK_WRAP_PYTHON AND PYTHON_VERSION_STRING VERSION_LESS 3.0)
      set(_wrap_module vtkWrappingPythonCore)
    endif()
    vtk_module_config(ITKVtkGlue_VTK
      vtkRenderingFreeType
      ${_target_freetypeopengl}
      ${_rendering_modules}
      vtkIOImage
      vtkIOLegacy
      vtkImagingSources
      ${_wrap_module}
      )

    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS \${ITKVtkGlue_VTK_DEFINITIONS})
  endif()

  # Artifacts of the VTKPythonPackage build configuration
  list(FILTER ITKVtkGlue_VTK_LIBRARIES EXCLUDE REGEX
    \"libpython-not-needed-symbols\")
  list(FILTER ITKVtkGlue_VTK_LIBRARIES EXCLUDE REGEX
    \"optimized\")
  list(FILTER ITKVtkGlue_VTK_LIBRARIES EXCLUDE REGEX
    \"debug\")
endif()
")

# For portable Python packages
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/targetLinkLibrariesWithDynamicLookup.cmake)

if(NOT ITK_SOURCE_DIR)
  find_package(ITK REQUIRED)
  list(APPEND CMAKE_MODULE_PATH ${ITK_CMAKE_DIR})
  include(ITKModuleExternal)
else()
  itk_module_impl()
endif()
