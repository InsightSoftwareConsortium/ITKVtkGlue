ITKVtkGlue
==========

.. |CircleCI| image:: https://circleci.com/gh/InsightSoftwareConsortium/ITKVTKGlue.svg?style=shield
    :target: https://circleci.com/gh/InsightSoftwareConsortium/ITKVTKGlue

.. |TravisCI| image:: https://travis-ci.org/InsightSoftwareConsortium/ITKVTKGlue.svg?branch=master
    :target: https://travis-ci.org/InsightSoftwareConsortium/ITKVTKGlue

.. |AppVeyor| image:: https://img.shields.io/appveyor/ci/itkrobot/itkvtkglue.svg
    :target: https://ci.appveyor.com/project/itkrobot/itkvtkglue

=========== =========== ===========
   Linux      macOS       Windows
=========== =========== ===========
|CircleCI|  |TravisCI|  |AppVeyor|
=========== =========== ===========

This module contains classes that bridge and ITK and VTK image processing and visualization pipeline.

ITK is an open-source, cross-platform library that provides developers with an extensive suite of software tools for image analysis. VTK enables visualization of images in three dimension. This module converts ITK and VTK image data structures in a processing pipelining without copying the pixel buffer.

For examples, see:

- `Convert An itk::Image To vtkImageData <https://itk.org/ITKExamples/src/Bridge/VtkGlue/ConvertAnitkImageTovtkImageData/Documentation.html>`_
- `Convert vtkImageData to an itk::Image <https://itk.org/ITKExamples/src/Bridge/VtkGlue/ConvertvtkImageDataToAnitkImage/Documentation.html>`_
- `Convert an RGB itk::Image To vtkImageData <https://itk.org/ITKExamples/src/Bridge/VtkGlue/ConvertAnRGBitkImageTovtkImageData/Documentation.html>`_
- `Convert RGB vtkImageData to an itk::Image <https://itk.org/ITKExamples/src/Bridge/VtkGlue/ConvertRGBvtkImageDataToAnitkImage/Documentation.html>`_
