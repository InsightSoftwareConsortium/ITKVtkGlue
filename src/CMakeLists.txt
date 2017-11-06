set(ITKVtkGlue_SRCS
  itkMinimalStandardRandomVariateGenerator.cxx
  )

add_library(ITKVtkGlue ${ITKVtkGlue_SRCS})
itk_module_link_dependencies()
itk_module_target(ITKVtkGlue)
