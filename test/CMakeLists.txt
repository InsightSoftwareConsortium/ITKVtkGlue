itk_module_test()

set(ITKVtkGlueTests
  itkImageToVTKImageFilterTest.cxx
  itkImageToVTKImageFilterRGBTest.cxx
  itkVTKImageToImageFilterTest.cxx
  itkMeshToVTKUnstructuredGridFilterTest.cxx
  itkBoxSpatialObjectToVTKPolyDataFilterTest.cxx
  )
if(NOT VTK_RENDERING_BACKEND STREQUAL "None")
  # ### runViewImage ###
  # No test generated. Use it for view images (2D or 3D) with:
  # ITKVtkGlueTestDriver runViewImage image_file
  # or with a string to change title window, and change size of window
  # ITKVtkGlueTestDriver runViewImage image_file "MyImage"
  list(APPEND ITKVtkGlueTests
    runViewImage.cxx
    QuickViewTest.cxx
    itkVtkConnectedComponentImageFilterTest.cxx
    itkVtkMedianFilterTest.cxx
    )
endif()

CreateTestDriver(ITKVtkGlue "${ITKVtkGlue-Test_LIBRARIES}" "${ITKVtkGlueTests}")
target_link_libraries_with_dynamic_lookup(ITKVtkGlueTestDriver ${ITKVtkGlue_VTK_LIBRARIES})


function(itkvtk_add_test)
	set(options OPTIONAL )
	set(oneValueArgs NAME)
	set(multiValueArgs COMMAND)
	cmake_parse_arguments(ITKVTK "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
	itk_add_test(
	  NAME ${ITKVTK_NAME}
	  COMMAND ${ITKVTK_COMMAND})
	# Get VTK library (DLL for Windows) location
	set_tests_properties(${ITKVTK_NAME} PROPERTIES
		ENVIRONMENT PATH=$<TARGET_FILE_DIR:vtksys>)
endfunction()

itkvtk_add_test(
  NAME itkImageToVTKImageFilterTest
  COMMAND ITKVtkGlueTestDriver
    itkImageToVTKImageFilterTest
  )

itkvtk_add_test(
  NAME itkImageToVTKImageFilterRGBTest
  COMMAND ITKVtkGlueTestDriver
    itkImageToVTKImageFilterRGBTest DATA{Input/VisibleWomanEyeSlice.png}
  )


itkvtk_add_test(
  NAME itkVTKImageToImageFilterTest
  COMMAND ITKVtkGlueTestDriver
    itkVTKImageToImageFilterTest
  )

itkvtk_add_test(
  NAME itkMeshToVTKUnstructuredGridFilterTest
  COMMAND ITKVtkGlueTestDriver
  itkMeshToVTKUnstructuredGridFilterTest DATA{Input/cow.vtk} ${ITK_TEST_OUTPUT_DIR}/cow_converted.vtk DATA{Input/cow.vtk}
  )

itkvtk_add_test(NAME itkBoxSpatialObjectToVTKPolyDataFilterTest
  COMMAND ITKVtkGlueTestDriver
  itkBoxSpatialObjectToVTKPolyDataFilterTest ${ITK_TEST_OUTPUT_DIR}/BoxSpatialObject.vtk DATA{Baseline/BoxSpatialObject.vtk}
  )

if(NOT VTK_RENDERING_BACKEND STREQUAL "None")
  itkvtk_add_test(NAME itkVtkMedianImageFilterTest
    COMMAND ITKVtkGlueTestDriver
      itkVtkMedianFilterTest DATA{Input/cthead1.png} 2
    )
  set_property(TEST itkVtkMedianImageFilterTest APPEND PROPERTY LABELS REQUIRES_DISPLAY)

  itkvtk_add_test(NAME QuickViewTest
    COMMAND ITKVtkGlueTestDriver
             --compare DATA{Baseline/QuickViewTest.png,:}
                       ${ITK_TEST_OUTPUT_DIR}/QuickViewTest0.png
             --compareNumberOfPixelsTolerance 1500
      QuickViewTest DATA{Input/peppers.png} ${ITK_TEST_OUTPUT_DIR}
    )
  set_property(TEST QuickViewTest APPEND PROPERTY LABELS REQUIRES_DISPLAY)

  itkvtk_add_test(NAME itkVtkConnectedComponentImageFilterTest
    COMMAND ITKVtkGlueTestDriver
      itkVtkConnectedComponentImageFilterTest DATA{Input/cthead1.png}
    )
  set_property(TEST itkVtkConnectedComponentImageFilterTest APPEND PROPERTY LABELS REQUIRES_DISPLAY)
endif()
