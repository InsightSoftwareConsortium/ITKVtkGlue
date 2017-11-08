set(ITKVtkGlue_SRCS
  itkVTKImageExportBase.cxx
  QuickView.cxx
  )

itk_module_add_library(ITKVtkGlue ${ITKVtkGlue_SRCS})
target_link_libraries_with_dynamic_lookup(ITKVtkGlue ${ITKVtkGlue_VTK_LIBRARIES})
