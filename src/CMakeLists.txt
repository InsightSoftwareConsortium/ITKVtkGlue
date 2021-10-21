if(NOT VTK_RENDERING_BACKEND STREQUAL "None")
  set(ITKVtkGlue_SRCS
    QuickView.cxx
    itkVTKImageExportBase.cxx
    vtkPolyDataBaselineComparison.cxx
    )

  add_library(ITKVtkGlue ${ITKVtkGlue_SRCS})
  itk_module_link_dependencies()
  target_link_libraries_with_dynamic_lookup(ITKVtkGlue ${ITKVtkGlue_VTK_LIBRARIES})
  itk_module_target(ITKVtkGlue)
endif()
