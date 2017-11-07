set(ITKVtkGlue_SRCS
  itkVTKImageExportBase.cxx
  QuickView.cxx
  )

# VTK 5.8 and greater defines VTK_LIBRARIES
if(NOT VTK_LIBRARIES)
  set(ITKVtkGlue_VTK_LIBRARIES_EXPLICIT vtkIO;vtkHybrid;vtkVolumeRendering)
endif()

itk_module_add_library(ITKVtkGlue ${ITKVtkGlue_SRCS})
target_link_libraries(ITKVtkGlue LINK_PUBLIC ${ITKVtkGlue_VTK_LIBRARIES} ${ITKVtkGlue_VTK_LIBRARIES_EXPLICIT})
