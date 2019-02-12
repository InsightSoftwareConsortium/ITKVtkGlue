# the top-level README is used for describing this module, just
# re-used it for documentation here
get_filename_component(MY_CURRENT_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
file(READ "${MY_CURRENT_DIR}/README.rst" DOCUMENTATION)

# define the dependencies of the include module and the tests
itk_module(ITKVtkGlue
  ENABLE_SHARED
  DEPENDS
    ITKCommon
    ITKVTK
  COMPILE_DEPENDS
    ITKSpatialObjects
    ITKImageIntensity
  TEST_DEPENDS
    ITKTestKernel
    ITKSmoothing
    ITKLabelMap
    ITKImageFusion
    ITKStatistics
    ITKIOMeshVTK
    ITKIOSpatialObjects
  EXCLUDE_FROM_DEFAULT
  DESCRIPTION
    "${DOCUMENTATION}")

# extra test dependency on Smoothing is introduced by itkVtkMedianImagefilterTest.
# extra test dependency on ImageCompose is introduced by QuickViewTest.
# extra test dependency on LabelMap and ImageFusion is introduced by itkVtkConnectedComponentImageFilterTest
# extra test dependency on Statistics is introduced by itkVtkConnectedComponentImageFilterTest
