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

#include "itkTubeSpatialObjectToVTKPolyDataFilter.h"

#include "vtkPolyDataWriter.h"
#include "vtkPolyDataReader.h"
#include "vtkNew.h"

#include "itkVesselTubeSpatialObject.h"
#include "itkVesselTubeSpatialObjectPoint.h"
#include "itkTubeSpatialObject.h"
#include "itkSpatialObjectReader.h"

#include "vtkPolyDataBaselineComparison.h"

int itkTubeSpatialObjectToVTKPolyDataFilterTest(int argc, char *argv[])
{
  if( argc < 3 )
    {
    std::cerr << "Usage: " << argv[0] << " <InputTube> <OutputPolyData> <BaselinePolyData>" << std::endl;
    return EXIT_FAILURE;
    }
  const char * inputTubeFileName = argv[1];
  const char * outputPolyDataFileName = argv[2];
  const char * baselinePolyDataFileName = argv[3];

  constexpr unsigned int Dimension = 3;

  using SpatialObjectType = itk::SpatialObject< Dimension >;

  using TubeReaderType = itk::SpatialObjectReader< Dimension >;
  TubeReaderType::Pointer tubeReader = TubeReaderType::New();
  tubeReader->SetFileName( inputTubeFileName );
  tubeReader->Update();
  TubeReaderType::GroupPointer groupSpatialObject = tubeReader->GetGroup();
  const unsigned int numberOfChildren = groupSpatialObject->GetNumberOfChildren();
  std::cout << "Number of children: " << numberOfChildren << std::endl;
  SpatialObjectType::ChildrenListType * children = groupSpatialObject->GetChildren();

  using TubePointType = itk::VesselTubeSpatialObjectPoint< Dimension >;
  using TubeSpatialObjectType = itk::TubeSpatialObject< Dimension, itk::VesselTubeSpatialObjectPoint< Dimension >  >;
  TubeSpatialObjectType::Pointer tube = dynamic_cast< TubeSpatialObjectType * >( children->front().GetPointer() );
  delete children;

  using TransformType = SpatialObjectType::TransformType;
  TransformType * transform = tube->GetModifiableObjectToWorldTransform();
  TransformType::OutputVectorType axis;
  axis.Fill( 0.0 );
  axis[2] = 1.0;
  transform->Rotate3D( axis, 30.0 * itk::Math::pi / 180 );
  TransformType::OutputVectorType translation;
  translation[0] = 1.0;
  translation[1] = 2.0;
  translation[2] = 3.0;
  transform->Translate( translation );

  tube->ComputeLocalBoundingBox();
  tube->Print( std::cout );

  using ConverterType = itk::TubeSpatialObjectToVTKPolyDataFilter< TubeSpatialObjectType >;
  ConverterType::Pointer converter = ConverterType::New();
  converter->SetInput( tube );

  converter->Update();
  converter->Print( std::cout );

  vtkNew< vtkPolyDataWriter > polyDataWriter;
  polyDataWriter->SetInputData( converter->GetOutput() );
  polyDataWriter->SetFileName( outputPolyDataFileName );
  polyDataWriter->Update();

  //vtkNew< vtkPolyDataReader > polyDataReader;
  //polyDataReader->SetFileName( baselinePolyDataFileName );
  //polyDataReader->Update();

  //if( ! vtkPolyDataBaselineComparison( converter->GetOutput(), polyDataReader->GetOutput() ) )
    //{
    //std::cerr << "Generated PolyData did not match baseline!" << std::endl;
    //return EXIT_FAILURE;
    //}

  return EXIT_SUCCESS;
}
