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

#include "itkBoxSpatialObjectToVTKPolyDataFilter.h"

#include "vtkPolyDataWriter.h"
#include "vtkPolyDataReader.h"
#include "vtkNew.h"

#include "itkBoxSpatialObject.h"

#include "vtkPolyDataBaselineComparison.h"

int itkBoxSpatialObjectToVTKPolyDataFilterTest(int argc, char *argv[])
{
  if( argc < 3 )
    {
    std::cerr << "Usage: " << argv[0] << " <OutputPolyData> <BaselinePolyData>" << std::endl;
    return EXIT_FAILURE;
    }
  const char * outputPolyDataFileName = argv[1];
  const char * baselinePolyDataFileName = argv[2];

  using PixelType = unsigned char;
  constexpr unsigned int Dimension = 3;

  using SpatialObjectType = itk::BoxSpatialObject< Dimension >;
  SpatialObjectType::Pointer spatialObject = SpatialObjectType::New();
  SpatialObjectType::SizeType size;
  size[0] = 1.3;
  size[1] = 2.2;
  size[2] = 3.8;
  spatialObject->SetSize( size );

  using TransformType = SpatialObjectType::TransformType;
  TransformType * transform = spatialObject->GetModifiableObjectToWorldTransform();
  TransformType::OutputVectorType axis;
  axis.Fill( 0.0 );
  axis[2] = 1.0;
  transform->Rotate3D( axis, 30.0 * itk::Math::pi / 180 );
  TransformType::OutputVectorType translation;
  translation[0] = 1.0;
  translation[1] = 2.0;
  translation[2] = 3.0;
  transform->Translate( translation );

  spatialObject->ComputeLocalBoundingBox();
  spatialObject->Print( std::cout );

  using ConverterType = itk::BoxSpatialObjectToVTKPolyDataFilter< Dimension >;
  ConverterType::Pointer converter = ConverterType::New();
  converter->SetInput( spatialObject );

  converter->Update();
  converter->Print( std::cout );

  vtkNew< vtkPolyDataWriter > polyDataWriter;
  polyDataWriter->SetInputData( converter->GetOutput() );
  polyDataWriter->SetFileName( outputPolyDataFileName );
  polyDataWriter->Update();

  vtkNew< vtkPolyDataReader > polyDataReader;
  polyDataReader->SetFileName( baselinePolyDataFileName );
  polyDataReader->Update();

  if( ! vtkPolyDataBaselineComparison( converter->GetOutput(), polyDataReader->GetOutput() ) )
    {
    std::cerr << "Generated PolyData did not match baseline!" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
