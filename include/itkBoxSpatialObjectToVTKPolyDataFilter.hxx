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
#ifndef itkBoxSpatialObjectToVTKPolyDataFilter_hxx
#define itkBoxSpatialObjectToVTKPolyDataFilter_hxx

#include "itkBoxSpatialObjectToVTKPolyDataFilter.h"

#include "vtkCubeSource.h"

namespace itk
{

template< unsigned int VDimension >
BoxSpatialObjectToVTKPolyDataFilter<VDimension>
::BoxSpatialObjectToVTKPolyDataFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
}


template< unsigned int VDimension >
BoxSpatialObjectToVTKPolyDataFilter<VDimension>
::~BoxSpatialObjectToVTKPolyDataFilter()
{
}


template< unsigned int VDimension >
void
BoxSpatialObjectToVTKPolyDataFilter<VDimension>
::SetInput( const SpatialObjectType * input )
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput( 0, const_cast< SpatialObjectType * >( input ) );
}


template< unsigned int VDimension >
const typename BoxSpatialObjectToVTKPolyDataFilter<VDimension>::SpatialObjectType *
BoxSpatialObjectToVTKPolyDataFilter<VDimension>
::GetInput() const
{
  return itkDynamicCastInDebugMode< const SpatialObjectType * >( this->GetPrimaryInput() );
}


template< unsigned int VDimension >
const typename BoxSpatialObjectToVTKPolyDataFilter<VDimension>::SpatialObjectType *
BoxSpatialObjectToVTKPolyDataFilter<VDimension>
::GetInput( unsigned int idx ) const
{
  return dynamic_cast< const SpatialObjectType * >( this->ProcessObject::GetInput(idx) );
}


template< unsigned int VDimension >
void
BoxSpatialObjectToVTKPolyDataFilter<VDimension>
::GenerateData()
{
  const SpatialObjectType * spatialObject = this->GetInput();

  this->m_PolyData = vtkSmartPointer< vtkPolyData >::New();

  vtkNew< vtkCubeSource > cubeSource;
  for (unsigned int dim = 0; dim < Dimension; ++dim )
    {
      switch (dim) {
      case 0:
        cubeSource->SetXLength(spatialObject->GetSizeInObjectSpace()[0]);
        break;
      case 1:
        cubeSource->SetYLength(spatialObject->GetSizeInObjectSpace()[1]);
        break;
      case 2:
        cubeSource->SetZLength(spatialObject->GetSizeInObjectSpace()[2]);
        break;
      default:
        itkExceptionMacro("Unsupported dimension for SpatialObject conversion.");
      }
    }
  cubeSource->Update();
  this->m_PolyData = cubeSource->GetOutput();
  this->ApplyObjectToWorldTransformToPolyData();

}

} // end namespace itk

#endif
