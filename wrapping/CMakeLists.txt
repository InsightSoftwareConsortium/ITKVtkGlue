itk_wrap_module(ITKVtkGlue)
  if(PYTHON_VERSION_STRING VERSION_GREATER 3.0 AND "${VTK_VERSION}" VERSION_LESS 7.0.0)
    message(WARNING
      "The ITKVtkGlue module can only built with Python 3 and VTK >= 7.\n"
      "Only the c++ version of the module will be built.")
  endif()
  list(APPEND WRAPPER_SWIG_LIBRARY_FILES "${CMAKE_CURRENT_SOURCE_DIR}/VtkGlue.i")
  set(WRAPPER_LIBRARY_DEPENDS ITKCommon)

  set(WRAPPER_LIBRARY_LINK_LIBRARIES ${ITK_LIBRARIES})

  itk_auto_load_submodules()
  # We need to ensure that the VTK libraries are loaded, which are used by this
  # module, but with weak linking. Since VTK does not use lazy loading yet,
  # just 'import vtk' should do the trick.
  set(ITK_WRAP_PYTHON_LIBRARY_IMPORTS "import vtk\n${ITK_WRAP_PYTHON_LIBRARY_IMPORTS}")
itk_end_wrap_module()

if(${VTK_VERSION} VERSION_LESS 6.0.0)
  target_link_libraries_with_dynamic_lookup(ITKVtkGluePython vtkImagingPythonD vtkPythonCore)
elseif(TARGET vtkImagingKitPythonD)
  target_link_libraries_with_dynamic_lookup(ITKVtkGluePython vtkImagingKitPythonD)
else()
  target_link_libraries_with_dynamic_lookup(ITKVtkGluePython vtkImagingCorePythonD)
endif()

