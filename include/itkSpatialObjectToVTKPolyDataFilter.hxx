/*=========================================================================
 *
 *  Copyright NumFOCUS
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
#ifndef itkSpatialObjectToVTKPolyDataFilter_hxx
#define itkSpatialObjectToVTKPolyDataFilter_hxx

#include "itkSpatialObjectToVTKPolyDataFilter.h"

#include "vtkTransform.h"
#include "vtkMatrix4x4.h"
#include "vtkTransformPolyDataFilter.h"

namespace itk
{

template< unsigned int VDimension >
SpatialObjectToVTKPolyDataFilter<VDimension>
::SpatialObjectToVTKPolyDataFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
}


template< unsigned int VDimension >
SpatialObjectToVTKPolyDataFilter<VDimension>
::~SpatialObjectToVTKPolyDataFilter()
{
}


template< unsigned int VDimension >
void
SpatialObjectToVTKPolyDataFilter<VDimension>
::SetInput( const SpatialObjectType * input )
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput( 0, const_cast< SpatialObjectType * >( input ) );
}


template< unsigned int VDimension >
const typename SpatialObjectToVTKPolyDataFilter<VDimension>::SpatialObjectType *
SpatialObjectToVTKPolyDataFilter<VDimension>
::GetInput() const
{
  return itkDynamicCastInDebugMode< const SpatialObjectType * >( this->GetPrimaryInput() );
}


template< unsigned int VDimension >
const typename SpatialObjectToVTKPolyDataFilter<VDimension>::SpatialObjectType *
SpatialObjectToVTKPolyDataFilter<VDimension>
::GetInput( unsigned int idx ) const
{
  return dynamic_cast< const SpatialObjectType * >( this->ProcessObject::GetInput(idx) );
}


template< unsigned int VDimension >
vtkPolyData *
SpatialObjectToVTKPolyDataFilter<VDimension>
::GetOutput()
{
  return this->m_PolyData.GetPointer();
}


template< unsigned int VDimension >
void
SpatialObjectToVTKPolyDataFilter<VDimension>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::Superclass::PrintSelf(os, indent);

  os << indent << "PolyData output: ";
    this->m_PolyData->Print( os );
}


template< unsigned int VDimension >
void
SpatialObjectToVTKPolyDataFilter<VDimension>
::Update()
{
  if( this->GetInput()->GetMTime() > this->m_ProcessedInputMTime )
    {
    const SpatialObjectType * input = this->GetInput();
    this->GenerateData();
    this->m_ProcessedInputMTime = input->GetMTime();
    }
}


template< unsigned int VDimension >
void
SpatialObjectToVTKPolyDataFilter<VDimension>
::ApplyObjectToWorldTransformToPolyData()
{
  const SpatialObjectType * spatialObject = this->GetInput();

  using TransformType = typename SpatialObjectType::TransformType;

  const typename SpatialObjectType::TransformType * transform = spatialObject->GetObjectToWorldTransform();

  const typename TransformType::MatrixType matrix = transform->GetMatrix();
  const typename TransformType::OffsetType offset = transform->GetOffset();

  vtkNew< vtkMatrix4x4 > matrixVTK;
  for( unsigned int ii = 0; ii < Dimension; ++ii )
    {
    for( unsigned int jj = 0; jj < Dimension; ++jj )
      {
      matrixVTK->SetElement( ii, jj, matrix[ii][jj] );
      }
    matrixVTK->SetElement( ii, 3, offset[ii] );
    }
  vtkNew< vtkTransform > transformVTK;
  transformVTK->SetMatrix( matrixVTK );

  vtkNew< vtkTransformPolyDataFilter > transformPolyDataFilter;
  transformPolyDataFilter->SetTransform( transformVTK );
  transformPolyDataFilter->SetInputData( this->m_PolyData );
  transformPolyDataFilter->Update();

  this->m_PolyData = transformPolyDataFilter->GetOutput();
}

} // end namespace itk

#endif
