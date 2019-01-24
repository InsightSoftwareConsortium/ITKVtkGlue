/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkViewImage_h
#define itkViewImage_h
#include <cstddef>
#include <string>
#include "vtkSmartPointer.h"
#include "vtkImagePlaneWidget.h"
#include "vtkRenderWindowInteractor.h"
#include "itkFixedArray.h"
namespace itk
{

/** \class ViewImage
 *
 * \brief Class with static function to visualize an image using just vtk.
 * It accepts 2D and 3D images.
 *
 * Usage example:
 *
 * itk::ViewImage<ImageType>::View( reader->GetOutput() );
 *
 * \ingroup ITKVtkGlue
 */
template< typename TImage >
class ViewImage : public ProcessObject
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(ViewImage);
  /** Standard class aliases. */
  using Self = ViewImage;
  using Superclass = ProcessObject;
  using Pointer = SmartPointer< Self >;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ViewImage, ProcessObject);

  using ImageType = TImage;
  /**
   * Static function to visualize 2D and 3D images.
   *
   * \param img Input image
   * \param windowTitle Title of the window.
   * \param windowWidth Width of the visualization window.
   * \param windowHeight Height of the visualization window.
   */
  static void View(const ImageType* img,
      const std::string& windowTitle = "itkView",
      size_t windowWidth = 600,
      size_t windowHeight = 600);

  /** Set/Get the image input of this viewer.  */
  using Superclass::SetInput;
  void SetInput(const ImageType *input);

  const ImageType * GetInput();

  const ImageType * GetInput(unsigned int idx);

  /** Update. */
  void Update() override;

  /** \brief Shows the entire image.
   *
   * Updates the pipeline.
   */
  void UpdateLargestPossibleRegion() override;


itkSetMacro(WindowWidth, size_t);
itkGetMacro(WindowWidth, size_t);
itkSetMacro(WindowHeight, size_t);
itkGetMacro(WindowHeight, size_t);
itkSetStringMacro(WindowTitle);
itkGetStringMacro(WindowTitle);
itkSetObjectMacro(RenderWindowInteractor, vtkRenderWindowInteractor);
itkGetObjectMacro(RenderWindowInteractor, vtkRenderWindowInteractor);

protected:
  ViewImage();
  ~ViewImage() override;
  void PrintSelf(std::ostream & os, Indent indent) const ITK_OVERRIDE;

  /** Does the real work. */
  void GenerateData() override;

private:
size_t m_WindowWidth{600};
size_t m_WindowHeight{600};
std::string m_WindowTitle{"itkView"};
vtkSmartPointer<vtkRenderWindowInteractor> m_RenderWindowInteractor;
FixedArray< vtkSmartPointer< vtkImagePlaneWidget >, 3 > m_SlicePlanes;
};
}// namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkViewImage.hxx"
#endif
#endif
