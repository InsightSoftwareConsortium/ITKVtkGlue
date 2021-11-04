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
    itkVTKVisualizeLevelSetsInteractivePauseTest.cxx
    itkVTKVisualize2DDenseImageLevelSetTest.cxx
    itkVTKVisualize2DWhitakerLevelSetTest.cxx
    itkImageToRGBVTKImageFilterTest.cxx
    itkVTKVisualize2DCellsLevelSetTest.cxx
    itkVTKVisualize2DCellsLevelSetSurfaceTest.cxx
    itkVTKVisualize2DCellsLevelSetLayersTest.cxx
    itkVTKVisualize2DWhitakerLevelSetLayersTest.cxx
    itkVTKVisualize2DShiLevelSetLayersTest.cxx
    itkVTKVisualize2DMalcolmLevelSetLayersTest.cxx
    itkVTKVisualize3DDenseImageLevelSetTest.cxx
    itkVTKVisualize3DWhitakerLevelSetTest.cxx
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
		ENVIRONMENT PATH=$<TARGET_FILE_DIR:VTK::vtksys>)
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

  set(TEMP ${ITK_TEST_OUTPUT_DIR})

  itk_add_test(NAME itkVTKVisualize2DDenseImageLevelSetTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DDenseImageLevelSetTest)

  itk_add_test(NAME itkVTKVisualize2DWhitakerLevelSetTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DWhitakerLevelSetTest)

  itk_add_test(NAME itkImageToRGBVTKImageFilterTest
    COMMAND ITKVtkGlueTestDriver itkImageToRGBVTKImageFilterTest)

  itk_add_test(NAME itkVTKVisualize2DWhitakerLevelSetLayersTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DWhitakerLevelSetLayersTest)

  itk_add_test(NAME itkVTKVisualize2DShiLevelSetLayersTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DShiLevelSetLayersTest)

  itk_add_test(NAME itkVTKVisualize2DMalcolmLevelSetLayersTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DMalcolmLevelSetLayersTest)

  itk_add_test(NAME itkVTKVisualize3DDenseImageLevelSetTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize3DDenseImageLevelSetTest)

  itk_add_test(NAME itkVTKVisualize3DWhitakerLevelSetTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize3DWhitakerLevelSetTest)

  itk_add_test(NAME itkVTKVisualize2DCellsDenseLevelSetTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DCellsLevelSetTest
    DATA{Input/cells.png} 60 Dense
    ${TEMP}/itkVTKVisualize2DCellsDenseLevelSetTest.png
    )
  itk_add_test(NAME itkVTKVisualize2DCellsDenseLevelSetSurfaceTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DCellsLevelSetSurfaceTest
    DATA{Input/cells.png} 60 Dense
    ${TEMP}/itkVTKVisualize2DCellsDenseLevelSetSurfaceTest.png
    )

  itk_add_test(NAME itkVTKVisualize2DCellsWhitakerLevelSetTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DCellsLevelSetTest
    DATA{Input/cells.png} 200 Whitaker
    ${TEMP}/itkVTKVisualize2DCellsWhitakerLevelSetTest.png
    )
  itk_add_test(NAME itkVTKVisualize2DCellsWhitakerLevelSetSurfaceTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DCellsLevelSetSurfaceTest
    DATA{Input/cells.png} 200 Whitaker
    ${TEMP}/itkVTKVisualize2DCellsWhitakerLevelSetSurfaceTest.png
    )
  itk_add_test(NAME itkVTKVisualize2DCellsWhitakerLevelSetLayersTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DCellsLevelSetLayersTest
    DATA{Input/cells.png} 200 Whitaker
    )

  itk_add_test(NAME itkVTKVisualize2DCellsShiLevelSetTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DCellsLevelSetTest
    DATA{Input/cells.png} 200 Shi
    ${TEMP}/itkVTKVisualize2DCellsShiLevelSetTest.png
    )
  itk_add_test(NAME itkVTKVisualize2DCellsShiLevelSetSurfaceTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DCellsLevelSetSurfaceTest
    DATA{Input/cells.png} 200 Shi
    ${TEMP}/itkVTKVisualize2DCellsShiLevelSetSurfaceTest.png
    )
  itk_add_test(NAME itkVTKVisualize2DCellsShiLevelSetLayersTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DCellsLevelSetLayersTest
    DATA{Input/cells.png} 200 Shi
    )

  itk_add_test(NAME itkVTKVisualize2DCellsMalcolmLevelSetTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DCellsLevelSetTest
    DATA{Input/cells.png} 60 Malcolm
    ${TEMP}/itkVTKVisualize2DCellsMalcolmLevelSetTest.png
    )
  itk_add_test(NAME itkVTKVisualize2DCellsMalcolmLevelSetSurfaceTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DCellsLevelSetSurfaceTest
    DATA{Input/cells.png} 60 Malcolm
    ${TEMP}/itkVTKVisualize2DCellsMalcolmLevelSetSurfaceTest.png
    )
  itk_add_test(NAME itkVTKVisualize2DCellsMalcolmLevelSetLayersTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DCellsLevelSetLayersTest
    DATA{Input/cells.png} 60 Malcolm
    )

  itk_add_test(NAME itkVTKVisualizeLevelSetsInteractivePauseTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualizeLevelSetsInteractivePauseTest
    DATA{Input/cells.png} 30
    )

  itk_add_test(NAME itkVTKVisualize2DLevelSetAsElevationMapTest
    COMMAND ITKVtkGlueTestDriver itkVTKVisualize2DLevelSetAsElevationMapTest
    )

  set_property(TEST
    itkVTKVisualize2DDenseImageLevelSetTest
    itkVTKVisualize2DWhitakerLevelSetTest
    itkImageToRGBVTKImageFilterTest
    itkVTKVisualize2DWhitakerLevelSetLayersTest
    itkVTKVisualize2DShiLevelSetLayersTest
    itkVTKVisualize2DMalcolmLevelSetLayersTest
    itkVTKVisualize3DDenseImageLevelSetTest
    itkVTKVisualize3DWhitakerLevelSetTest
    itkVTKVisualize2DCellsDenseLevelSetTest
    itkVTKVisualize2DCellsDenseLevelSetSurfaceTest
    itkVTKVisualize2DCellsWhitakerLevelSetTest
    itkVTKVisualize2DCellsWhitakerLevelSetSurfaceTest
    itkVTKVisualize2DCellsWhitakerLevelSetLayersTest
    itkVTKVisualize2DCellsShiLevelSetTest
    itkVTKVisualize2DCellsShiLevelSetSurfaceTest
    itkVTKVisualize2DCellsShiLevelSetLayersTest
    itkVTKVisualize2DCellsMalcolmLevelSetTest
    itkVTKVisualize2DCellsMalcolmLevelSetSurfaceTest
    itkVTKVisualize2DCellsMalcolmLevelSetLayersTest
    itkVTKVisualizeLevelSetsInteractivePauseTest
    itkVTKVisualize2DLevelSetAsElevationMapTest
    APPEND PROPERTY LABELS REQUIRES_DISPLAY)
endif()
