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
#ifndef itkViewImage_hxx
#define itkViewImage_hxx

#include "vtkRenderWindow.h"

#include "vtkInteractorStyleRubberBand3D.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"

#include "itkImage.h"
#include "itkImageToVTKImageFilter.h"
#include "itkStatisticsImageFilter.h"

#include "itkViewImage.h"

namespace itk
{

template< typename TImage >
void
ViewImage< TImage >
::View( const ImageType* input,
        const std::string& winTitle,
        size_t winWidth,
        size_t winHeight)
{
    ViewImage<ImageType>::Pointer view = ViewImage<ImageType>::New();
    view->SetInput(input);
    view->SetWindowTitle(winTitle);
    view->SetWindowWidth(winWidth);
    view->SetWindowHeight(winHeight);
    view->Update();
}

//---------------------------------------------------------
template< typename TImage >
ViewImage< TImage >
::ViewImage() = default;

//---------------------------------------------------------
template< typename TImage >
ViewImage< TImage >
::~ViewImage() = default;

//---------------------------------------------------------
template< typename TImage >
void
ViewImage< TImage >
::SetInput(const ImageType *input)
{
  // ProcessObject is not const_correct so this cast is required here.
  this->ProcessObject::SetNthInput( 0,
                                    const_cast< ImageType * >( input ) );
}

//---------------------------------------------------------
template< typename TImage >
const typename ViewImage< TImage >::ImageType *
ViewImage< TImage >
::GetInput()
{
  return itkDynamicCastInDebugMode< ImageType * >( this->GetPrimaryInput() );
}

//---------------------------------------------------------
template< typename TImage >
const typename ViewImage< TImage >::ImageType *
ViewImage< TImage >
::GetInput(unsigned int idx)
{
  return itkDynamicCastInDebugMode< ImageType * >( this->ProcessObject::GetInput(idx) );
}


//---------------------------------------------------------
template< typename TImage >
void
ViewImage< TImage >
::GenerateData()
{
  const ImageType *input = this->GetInput();
  using ConnectorType = ImageToVTKImageFilter< ImageType >;
  auto connector = ConnectorType::New();
  connector->SetInput(input);
  connector->Update();
  connector->UpdateLargestPossibleRegion();

  // Setup renderers
  vtkSmartPointer< vtkRenderer > renderer = vtkSmartPointer< vtkRenderer >::New();

  vtkSmartPointer< vtkRenderWindow > renderWindow;
  // Setup render window interactor
  vtkSmartPointer< vtkRenderWindowInteractor > renderWindowInteractor;
  if(!m_RenderWindowInteractor)
  {
    // Setup render window
    renderWindow = vtkSmartPointer< vtkRenderWindow >::New();
    renderWindowInteractor = vtkSmartPointer< vtkRenderWindowInteractor >::New();
  }
  else
  {
    renderWindowInteractor = m_RenderWindowInteractor;
    renderWindow = renderWindowInteractor->GetRenderWindow();
  }
  renderWindow->SetWindowName(m_WindowTitle.c_str());
  renderWindow->SetSize(m_WindowWidth, m_WindowHeight);
  renderWindow->AddRenderer(renderer);

  vtkSmartPointer< vtkInteractorStyleRubberBand3D > style =
    vtkSmartPointer< vtkInteractorStyleRubberBand3D >::New();
  renderWindowInteractor->SetInteractorStyle(style);

  // Render and start interaction
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Prepare for slices.
  using FilterType = StatisticsImageFilter< ImageType >;
  auto filter = FilterType::New();
  filter->SetInput(input);
  filter->Update();
  filter->UpdateLargestPossibleRegion();
  double minIntensity = filter->GetMinimum();
  double maxIntensity = filter->GetMaximum();
  double window = maxIntensity - minIntensity;
  double level  = minIntensity + window / 2;
  /** SLICES */
  for ( unsigned i = 0; i < 3; ++i )
    {
    m_SlicePlanes[i] = vtkSmartPointer< vtkImagePlaneWidget >::New();
    m_SlicePlanes[i]->SetResliceInterpolateToCubic();
    m_SlicePlanes[i]->DisplayTextOn();
    m_SlicePlanes[i]->SetInteractor(renderWindowInteractor);
    m_SlicePlanes[i]->PlaceWidget();
    m_SlicePlanes[i]->SetSliceIndex(0);
    m_SlicePlanes[i]->SetMarginSizeX(0);
    m_SlicePlanes[i]->SetMarginSizeY(0);
    m_SlicePlanes[i]->SetRightButtonAction(
      vtkImagePlaneWidget::VTK_SLICE_MOTION_ACTION);
    m_SlicePlanes[i]->SetMiddleButtonAction(
      vtkImagePlaneWidget::VTK_WINDOW_LEVEL_ACTION);
    m_SlicePlanes[i]->TextureInterpolateOff();

    m_SlicePlanes[i]->SetInputData(connector->GetOutput());
    m_SlicePlanes[i]->SetPlaneOrientation(i);
    m_SlicePlanes[i]->UpdatePlacement();
    m_SlicePlanes[i]->SetWindowLevel(window, level);
    m_SlicePlanes[i]->On();
    }
  // Flip camera because VTK-ITK different corner for origin.
  double pos[3];
  double vup[3];
  vtkCamera *cam = renderer->GetActiveCamera();
  cam->GetPosition(pos);
  cam->GetViewUp(vup);
  for ( unsigned int i = 0; i < 3; ++i )
    {
    pos[i] = -pos[i];
    vup[i] = -vup[i];
    }
  cam->SetPosition(pos);
  cam->SetViewUp(vup);

  renderer->ResetCamera();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();
}

//---------------------------------------------------------
template< typename TImage >
void
ViewImage< TImage >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Window Title: "
     << ( m_WindowTitle.data() ? m_WindowTitle.data() : "(none)" ) << "\n";

  os << indent << "Window Width: " << m_WindowWidth << "\n";
  os << indent << "Window Height: " << m_WindowHeight << "\n";

  os << indent << "Render Window Interactor: ";
  if ( m_RenderWindowInteractor.GetPointer() == nullptr )
    {
    os << "(none)\n";
    }
  else
    {
    os << m_RenderWindowInteractor << "\n";
    }

  for(int i = 0; i <m_SlicePlanes.Size(); i++)
    {
    os << indent << "Slice Plane " << i << ":";
  if ( m_SlicePlanes[i].GetPointer() == nullptr )
    {
    os << "(none)\n";
    }
  else
    {
    os << m_SlicePlanes[i] << "\n";
    }
  }
}

template< typename TImage >
void
ViewImage< TImage >
::Update()
{
    this->GenerateData();
}

template< typename TImage >
void
ViewImage< TImage >
::UpdateLargestPossibleRegion()
{
    this->GenerateData();
}

}// namespace itk
#endif
