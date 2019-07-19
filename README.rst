ITKVTKGlue
==========

.. image:: https://dev.azure.com/ITKVtkGlue/ITKVtkGlue/_apis/build/status/InsightSoftwareConsortium.ITKVtkGlue?branchName=master
    :target: https://dev.azure.com/ITKVtkGlue/ITKVtkGlue/_build/latest?definitionId=1&branchName=master
    :alt: Build Status

This module contains classes that bridge and ITK and VTK image processing and visualization pipeline.

ITK is an open-source, cross-platform library that provides developers with an extensive suite of software tools for image analysis. VTK enables visualization of images in three dimension. This module converts ITK and VTK image data structures in a processing pipelining without copying the pixel buffer.

For examples, see:

- `Convert An itk::Image To vtkImageData <https://itk.org/ITKExamples/src/Bridge/VtkGlue/ConvertAnitkImageTovtkImageData/Documentation.html>`_
- `Convert vtkImageData to an itk::Image <https://itk.org/ITKExamples/src/Bridge/VtkGlue/ConvertvtkImageDataToAnitkImage/Documentation.html>`_
- `Convert an RGB itk::Image To vtkImageData <https://itk.org/ITKExamples/src/Bridge/VtkGlue/ConvertAnRGBitkImageTovtkImageData/Documentation.html>`_
- `Convert RGB vtkImageData to an itk::Image <https://itk.org/ITKExamples/src/Bridge/VtkGlue/ConvertRGBvtkImageDataToAnitkImage/Documentation.html>`_
