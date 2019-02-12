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
#ifndef itkTubeSpatialObjectToVTKPolyDataFilter_hxx
#define itkTubeSpatialObjectToVTKPolyDataFilter_hxx

#include "itkTubeSpatialObjectToVTKPolyDataFilter.h"

#include "vtkPolyLine.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkCleanPolyData.h"
#include "vtkTubeFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkSphereSource.h"
#include "vtkFloatArray.h"

namespace
{

/** Get the index of the lookup table given R,G,B values */
double
GetLookUpTableIndex(float r,float g,float b)
{
  // Define the Color Table
  //double ColorFactor = 1;
  //double ColorOffset = 0;

  int hueCase;
  float hue, sat, val, lx, ly, lz, frac, hinc, sinc, vinc;
  float rgba[4];
  float R,G,B;

  constexpr unsigned int NumberOfColors = 256;

  hinc = (0 - 0.6667)/(NumberOfColors-1);
  sinc = 0;
  vinc = 0;

  // We want to minimize the distance
  constexpr float distance = 999999999;
  unsigned int index = 0;

  for( unsigned int i = 0; i < NumberOfColors; ++i )
    {
    hue = 0.6667 + i * hinc;
    sat = 1 + i * sinc;
    val = 1 + i * vinc;

    hueCase = (int)(hue * 6);
    frac = 6*hue - hueCase;
    lx = val*(1.0 - sat);
    ly = val*(1.0 - sat*frac);
    lz = val*(1.0 - sat*(1.0 - frac));

    switch (hueCase)
    {
      // 0<hue<1/6
      case 0:
      case 6:
        rgba[0] = val;
        rgba[1] = lz;
        rgba[2] = lx;
        break;
      // 1/6<hue<2/6
      case 1:
        rgba[0] = ly;
        rgba[1] = val;
        rgba[2] = lx;
        break;
      // 2/6<hue<3/6
      case 2:
        rgba[0] = lx;
        rgba[1] = val;
        rgba[2] = lz;
        break;
      // 3/6<hue/4/6
      case 3:
        rgba[0] = lx;
        rgba[1] = ly;
        rgba[2] = val;
        break;
      // 4/6<hue<5/6
      case 4:
        rgba[0] = lz;
        rgba[1] = lx;
        rgba[2] = val;
        break;
      // 5/6<hue<1
      case 5:
        rgba[0] = val;
        rgba[1] = lx;
        rgba[2] = ly;
        break;
    }

    R = (unsigned char)
       ((float)127.5*(1.0+(float)cos((1.0-(double)rgba[0])*3.141593)));
    G = (unsigned char)
        ((float)127.5*(1.0+(float)cos((1.0-(double)rgba[1])*3.141593)));
    B = (unsigned char)
        ((float)127.5*(1.0+(float)cos((1.0-(double)rgba[2])*3.141593)));

    float Red = float(R)/255;
    float Green = float(G)/255;
    float Blue = float(B)/255;

    double dist = sqrt((Red-r)*(Red-r)+(Green-g)*(Green-g)+(Blue-b)*(Blue-b));

    if(dist < distance )
      {
      index =i;
      }
    }

  return double(0.99-(double(index))/256);
}

} // end anonymous namespace

namespace itk
{

template< typename TTube >
TubeSpatialObjectToVTKPolyDataFilter< TTube >
::TubeSpatialObjectToVTKPolyDataFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
}


template< typename TTube >
TubeSpatialObjectToVTKPolyDataFilter< TTube >
::~TubeSpatialObjectToVTKPolyDataFilter()
{
}


template< typename TTube >
void
TubeSpatialObjectToVTKPolyDataFilter< TTube >
::SetInput( const SpatialObjectType * input )
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput( 0, const_cast< SpatialObjectType * >( input ) );
}


template< typename TTube >
const typename TubeSpatialObjectToVTKPolyDataFilter<TTube>::SpatialObjectType *
TubeSpatialObjectToVTKPolyDataFilter< TTube >
::GetInput() const
{
  return itkDynamicCastInDebugMode< const SpatialObjectType * >( this->GetPrimaryInput() );
}


template< typename TTube >
const typename TubeSpatialObjectToVTKPolyDataFilter< TTube >::SpatialObjectType *
TubeSpatialObjectToVTKPolyDataFilter< TTube >
::GetInput( unsigned int idx ) const
{
  return dynamic_cast< const SpatialObjectType * >( this->ProcessObject::GetInput(idx) );
}


template< typename TTube >
void
TubeSpatialObjectToVTKPolyDataFilter< TTube >
::GenerateData()
{
  const SpatialObjectType * tube = this->GetInput();
  using TubeType = SpatialObjectType;

  this->m_PolyData = vtkSmartPointer< vtkPolyData >::New();

  //Step 1: Copy skeleton points from a vessel into vtkPoints
  // vtkPoints assumes a triplet is coming so use pointer arithmetic
  // to jump to the next spot in a multidimensional array
  const SizeValueType nPoints = tube->GetNumberOfPoints();

  vtkNew< vtkPoints > vPoints;
  vPoints->SetNumberOfPoints( nPoints );

  vtkNew< vtkFloatArray > vScalars;
  vScalars->SetNumberOfTuples( nPoints );

  vtkNew< vtkFloatArray > vColorScalars;
  if( this->m_UsePointColors )
    {
    vColorScalars->SetNumberOfTuples( nPoints );

    const typename TubeType::TubePointType* point = static_cast<const typename TubeType::TubePointType*>(tube->GetPoint(0));

    for( SizeValueType i = 0; i < nPoints; ++i )
      {
      point = static_cast<const typename TubeType::TubePointType*>(tube->GetPoint(i));
      const typename TubeType::TubePointType::PointType & position = point->GetPosition();
      vPoints->SetPoint(i,
        (float)position[0],
        (float)position[1],
        (float)position[2]);
      vScalars->SetTuple1(i, point->GetRadius()*0.95);

      const typename TubeType::TubePointType::Superclass::ColorType & color = point->GetColor();
      vColorScalars->SetTuple1(i,
        GetLookUpTableIndex(color[0],
          color[1],
          color[2])
        );
      }
    }

  // Step 2: create a point id list (for a polyline this is just linear)
  vtkIdType* pntIds = new vtkIdType[nPoints];
  for ( SizeValueType i = 0; i < nPoints; ++i )
    {
    pntIds[i] = i;
    }

  // Step3: create a polyline from the points and pt id list
  vtkNew< vtkPolyLine > vPLine;
  vPLine->Initialize( nPoints, pntIds, vPoints );

  // Step 4: convert this to a cellarray (required for input to polydata)
  vtkNew< vtkCellArray > vCA;
  vCA->InsertNextCell(vPLine);

  // Step 5: create a scalar array that indicates the radius at each
  // skeleton point. Vtk's way of setting radius is screwy: it fails if every
  // point has the same radius. It also uses a minimum radius  (called radius)
  // and a max radius (defined by scale factor). In order to get this to work,
  // you need to find the min and max of your vessel radii--if the same, later
  // set a constant radius in the tube filter. If not the same, you need to
  // define the min radius and the ratio max/min. If you send these params,
  // the tube will end up with proper radius settings. Weird.

  // Step 6: convert to polydata (required for input to anything else)
  vtkNew< vtkPolyData > vPData;
  vPData->SetLines(vCA);
  vPData->SetPoints(vPoints);
  float range[2];
  vScalars->GetValueRange(range);
  float minScalar = range[0];
  float maxScalar = range[1];
  if (minScalar <= 0.0001)
    {
    minScalar = 0.0001;
    }
  if(maxScalar < minScalar)
    {
    maxScalar = minScalar;
    }

  if( this->m_UsePointColors )
    {
    vPData->GetPointData()->SetScalars(vColorScalars);
    }

  // Step 7: remove any duplicate points from polydata. The tube filter
  // fails if any duplicates are present
  vtkNew< vtkCleanPolyData > vClean;
  vClean->SetInputData(vPData);

  //Step 8: Make tubes. The number of sides per tube is set by nsides.
  //Even an nsides of 3 looks surprisingly good.
  vtkNew< vtkTubeFilter > vTFilter;
  vTFilter->SetNumberOfSides(this->m_NumberOfSides);
  vTFilter->SetInputConnection(vClean->GetOutputPort());
  vTFilter->CappingOff();

  if( this->m_UsePointColors )
    {
    vTFilter->SetVaryRadiusToVaryRadiusByVector();
    vTFilter->SetRadius(minScalar);   //this call sets min rad. Weird.
    vTFilter->SetRadiusFactor(maxScalar/minScalar); //sets max rad. Weird
    }
  else
    {
    vTFilter->SetRadius(minScalar);
    vTFilter->SetVaryRadiusToVaryRadiusOff();
    }

  vTFilter->Update();
  this->m_PolyData = vTFilter->GetOutput();
  this->ApplyObjectToWorldTransformToPolyData();
}

} // end namespace itk

#endif
