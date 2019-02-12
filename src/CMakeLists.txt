set(ITKVtkGlue_SRCS
  itkVTKImageExportBase.cxx
  vtkPolyDataBaselineComparison.cxx
  )
if(NOT VTK_RENDERING_BACKEND STREQUAL "None")
  list(APPEND ITKVtkGlue_SRCS QuickView.cxx)
endif()


itk_module_add_library(ITKVtkGlue ${ITKVtkGlue_SRCS})
target_link_libraries_with_dynamic_lookup(ITKVtkGlue ${ITKVtkGlue_VTK_LIBRARIES})
