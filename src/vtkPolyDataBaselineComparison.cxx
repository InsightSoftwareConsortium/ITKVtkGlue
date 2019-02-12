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
#include "vtkPolyDataBaselineComparison.h"

// Compare two vtkPolyData instances and return true if they are roughly the
// same.
bool
vtkPolyDataBaselineComparison( vtkPolyData * input, vtkPolyData * baseline )
{
  bool same = true;
  if( input->GetNumberOfPoints() != baseline->GetNumberOfPoints() )
    {
    std::cerr << "NumberOfPoints are different!" << std::endl;
    std::cerr << "Input: " << input->GetNumberOfPoints()  << std::endl;
    std::cerr << "Baseline: " << baseline->GetNumberOfPoints()  << std::endl;
    same = false;
    }
  if( input->GetNumberOfPoints() == baseline->GetNumberOfPoints() )
    {
    const vtkIdType numberOfPoints = input->GetNumberOfPoints();
    double inputPoint[3];
    double baselinePoint[3];
    for( vtkIdType pointId = 0; pointId < numberOfPoints; ++pointId )
      {
      input->GetPoint( pointId, inputPoint );
      baseline->GetPoint( pointId, baselinePoint );
      for( unsigned int index = 0; index < 3; ++index )
        {
        if( ! itk::Math::FloatAlmostEqual( inputPoint[index], baselinePoint[index], 10, 1e-5 ) )
          {
          std::cerr << "Point difference: Id: " << pointId
                    << " Index: " << index
                    << " Value: " << inputPoint[index] << " versus " << baselinePoint[index] << std::endl;
          same = false;
          }
        }
      }
    }

  if( input->GetNumberOfVerts() != baseline->GetNumberOfVerts() )
    {
    std::cerr << "NumberOfVerts are different!" << std::endl;
    std::cerr << "Input: " << input->GetNumberOfVerts()  << std::endl;
    std::cerr << "Baseline: " << baseline->GetNumberOfVerts()  << std::endl;
    same = false;
    }
  if( input->GetNumberOfLines() != baseline->GetNumberOfLines() )
    {
    std::cerr << "NumberOfLines are different!" << std::endl;
    std::cerr << "Input: " << input->GetNumberOfLines()  << std::endl;
    std::cerr << "Baseline: " << baseline->GetNumberOfLines()  << std::endl;
    same = false;
    }
  if( input->GetNumberOfPolys() != baseline->GetNumberOfPolys() )
    {
    std::cerr << "NumberOfPolys are different!" << std::endl;
    std::cerr << "Input: " << input->GetNumberOfPolys()  << std::endl;
    std::cerr << "Baseline: " << baseline->GetNumberOfPolys()  << std::endl;
    same = false;
    }
  if( input->GetNumberOfPolys() != baseline->GetNumberOfPolys() )
    {
    std::cerr << "NumberOfPolys are different!" << std::endl;
    std::cerr << "Input: " << input->GetNumberOfPolys()  << std::endl;
    std::cerr << "Baseline: " << baseline->GetNumberOfPolys()  << std::endl;
    same = false;
    }

  return same;
}
