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

#include "itkMeshToVTKUnstructuredGridFilter.h"

#include "vtkPolyDataWriter.h"
#include "vtkPolyDataReader.h"
#include "vtkNew.h"
#include "vtkGeometryFilter.h"

#include "itkMeshFileReader.h"
#include "itkMesh.h"
#include "itkQuadEdgeMesh.h"

#include "vtkPolyDataBaselineComparison.h"

int itkMeshToVTKUnstructuredGridFilterTest(int argc, char *argv[])
{
  if( argc < 4 )
    {
    std::cerr << "Usage: " << argv[0] << " <InputMesh> <OutputPolyData> <BaselinePolyData>" << std::endl;
    return EXIT_FAILURE;
    }
  const char * inputMeshFileName = argv[1];
  const char * outputPolyDataFileName = argv[2];
  const char * baselinePolyDataFileName = argv[3];

  using PixelType = unsigned char;
  constexpr unsigned int Dimension = 3;
  using MeshType = itk::Mesh< PixelType, Dimension >;
  //using MeshType = itk::QuadEdgeMesh<PixelType, Dimension>;

  using MeshReaderType = itk::MeshFileReader< MeshType >;
  MeshReaderType::Pointer meshReader = MeshReaderType::New();
  meshReader->SetFileName( inputMeshFileName );
  meshReader->Update();

  using ConverterType = itk::MeshToVTKUnstructuredGridFilter< MeshType >;
  ConverterType::Pointer converter = ConverterType::New();
  converter->SetInput( meshReader->GetOutput() );

  converter->Update();

  converter->Print( std::cout );

  vtkNew< vtkGeometryFilter > geometryFilter;
  geometryFilter->SetInputData( converter->GetOutput() );

  vtkNew< vtkPolyDataWriter > polyDataWriter;
  polyDataWriter->SetInputConnection( geometryFilter->GetOutputPort() );
  polyDataWriter->SetFileName( outputPolyDataFileName );
  polyDataWriter->Update();

  vtkNew< vtkPolyDataReader > polyDataReader;
  polyDataReader->SetFileName( baselinePolyDataFileName );
  polyDataReader->Update();

  if( ! vtkPolyDataBaselineComparison( geometryFilter->GetOutput(), polyDataReader->GetOutput() ) )
    {
    std::cerr << "Generated PolyData did not match baseline!" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
