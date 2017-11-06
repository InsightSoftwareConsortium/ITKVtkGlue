itk_module_test()

set(ITKVtkGlueTests
  itkMinimalStandardRandomVariateGeneratorTest.cxx
  itkNormalDistributionImageSourceTest.cxx
  )

CreateTestDriver(ITKVtkGlue "${ITKVtkGlue_LIBRARIES}" "${ITKVtkGlueTests}")

itk_add_test(NAME itkMinimalStandardRandomVariateGeneratorTest
  COMMAND ITKVtkGlueTestDriver itkMinimalStandardRandomVariateGeneratorTest
  )

itk_add_test(NAME itkNormalDistributionImageSourceTest
  COMMAND ITKVtkGlueTestDriver --without-threads
  --compare
    ${ITK_TEST_OUTPUT_DIR}/itkNormalDistributionImageSourceTestOutput.mha
    DATA{Baseline/itkNormalDistributionImageSourceTestOutput.mha}
  itkNormalDistributionImageSourceTest
    ${ITK_TEST_OUTPUT_DIR}/itkNormalDistributionImageSourceTestOutput.mha
  )
