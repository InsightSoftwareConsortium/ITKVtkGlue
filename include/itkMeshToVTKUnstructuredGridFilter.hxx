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
#ifndef itkMeshToVTKUnstructuredGridFilter_hxx
#define itkMeshToVTKUnstructuredGridFilter_hxx

#include "itkMeshToVTKUnstructuredGridFilter.h"
#include <unordered_map>

#include "itkLineCell.h"
#include "itkMesh.h"
#include "itkTriangleCell.h"
#include "itkQuadrilateralCell.h"
#include "itkPolygonCell.h"
#include "itkTetrahedronCell.h"
#include "itkHexahedronCell.h"

#include <vtkCellArray.h>

namespace itk
{

template< typename TMesh >
class VisitVTKCellsClass
{
public:
  using MeshType = TMesh;
  // typedef the itk cells we are interested in
  using CellInterfaceType = itk::CellInterface< typename MeshType::PixelType, typename MeshType::CellTraits >;

  using LineCellType = itk::LineCell<CellInterfaceType>;
  using TriangleCellType = itk::TriangleCell<CellInterfaceType>;
  using QuadrilateralCellType = itk::QuadrilateralCell<CellInterfaceType>;
  using PolygonCellType = itk::PolygonCell<CellInterfaceType>;
  using TetrahedronCellType = itk::TetrahedronCell<CellInterfaceType>;
  using HexahedronCellType = itk::HexahedronCell<CellInterfaceType>;

  // Set the vtkCellArray that will be constructed
  void SetCellArray(vtkCellArray* a)
  {
    m_Cells = a;
  }

  // Set the cell counter pointer
  void SetCellCounter(int* i)
  {
    m_LastCell = i;
  }

  // Set the type array for storing the vtk cell types
  void SetTypeArray(int* i)
  {
    m_TypeArray = i;
  }

  // Visit a line and create the VTK_LINE cell
  void Visit(unsigned long, LineCellType* t)
  {
    m_Cells->InsertNextCell(2,  (vtkIdType*)t->PointIdsBegin());
    m_TypeArray[*m_LastCell] = VTK_LINE;
    (*m_LastCell)++;
  }

  // Visit a triangle and create the VTK_TRIANGLE cell
  void Visit(unsigned long, TriangleCellType* t)
  {
    m_Cells->InsertNextCell(3,  (vtkIdType*)t->PointIdsBegin());
    m_TypeArray[*m_LastCell] = VTK_TRIANGLE;
    (*m_LastCell)++;
  }

  // Visit a triangle and create the VTK_QUAD cell
  void Visit(unsigned long, QuadrilateralCellType* t)
  {
    m_Cells->InsertNextCell(4,  (vtkIdType*)t->PointIdsBegin());
    m_TypeArray[*m_LastCell] = VTK_QUAD;
    (*m_LastCell)++;
  }

  // Visit a triangle and create the VTK_POLYGON cell
  void Visit(unsigned long, PolygonCellType* t)
  {
    m_Cells->InsertNextCell(t->GetNumberOfPoints(),  (vtkIdType*)t->PointIdsBegin());
    m_TypeArray[*m_LastCell] = VTK_POLYGON;
    (*m_LastCell)++;
  }

  // Visit a triangle and create the VTK_POLYGON cell
  void Visit(unsigned long, TetrahedronCellType* t)
  {
    m_Cells->InsertNextCell(4,  (vtkIdType*)t->PointIdsBegin());
    m_TypeArray[*m_LastCell] = VTK_TETRA;
    (*m_LastCell)++;
  }

  // Visit a triangle and create the VTK_POLYGON cell
  void Visit(unsigned long, HexahedronCellType* t)
  {
    m_Cells->InsertNextCell(8,  (vtkIdType*)t->PointIdsBegin());
    m_TypeArray[*m_LastCell] = VTK_HEXAHEDRON;
    (*m_LastCell)++;
  }

private:
  vtkCellArray* m_Cells;
  int* m_LastCell;
  int* m_TypeArray;
};

}

namespace itk
{

template <typename TInputMesh>
MeshToVTKUnstructuredGridFilter<TInputMesh>
::MeshToVTKUnstructuredGridFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
}


template <typename TInputMesh>
MeshToVTKUnstructuredGridFilter<TInputMesh>
::~MeshToVTKUnstructuredGridFilter()
{
}


template <typename TInputMesh>
void
MeshToVTKUnstructuredGridFilter<TInputMesh>
::SetInput( const InputMeshType * input )
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput( 0,
                                    const_cast< TInputMesh * >( input ) );
}


template <typename TInputMesh>
const typename MeshToVTKUnstructuredGridFilter<TInputMesh>::InputMeshType *
MeshToVTKUnstructuredGridFilter<TInputMesh>
::GetInput() const
{
  return itkDynamicCastInDebugMode< const TInputMesh * >( this->GetPrimaryInput() );
}


template <typename TInputMesh>
const typename MeshToVTKUnstructuredGridFilter<TInputMesh>::InputMeshType *
MeshToVTKUnstructuredGridFilter<TInputMesh>
::GetInput( unsigned int idx ) const
{
  return dynamic_cast< const TInputMesh * >
         ( this->ProcessObject::GetInput(idx) );
}


template <typename TInputMesh>
vtkUnstructuredGrid *
MeshToVTKUnstructuredGridFilter<TInputMesh>
::GetOutput()
{
  return this->m_UnstructuredGrid.GetPointer();
}


template <typename TInputMesh>
void
MeshToVTKUnstructuredGridFilter<TInputMesh>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::Superclass::PrintSelf(os, indent);

  os << indent << "UnstructuredGrid output: ";
    this->m_UnstructuredGrid->Print( os );
}


template <typename TInputMesh>
void
MeshToVTKUnstructuredGridFilter<TInputMesh>
::Update()
{
  if( this->GetInput()->GetMTime() > this->m_ProcessedInputMTime )
    {
    InputMeshType * input = const_cast< InputMeshType * >( this->GetInput() );
    input->Update();
    this->GenerateData();
    this->m_ProcessedInputMTime = input->GetMTime();
    }
}


template <typename TInputMesh>
void
MeshToVTKUnstructuredGridFilter<TInputMesh>
::GenerateData()
{
  const InputMeshType * mesh = this->GetInput();

  this->m_UnstructuredGrid = vtkSmartPointer< vtkUnstructuredGrid >::New();

  // Get the number of points in the mesh
  const SizeValueType numPoints = mesh->GetNumberOfPoints();

  // Create the vtkPoints object and set the number of points
  vtkPoints* vpoints = vtkPoints::New();
  vpoints->SetNumberOfPoints(numPoints);
  // Iterate over all the points in the itk mesh filling in
  // the vtkPoints object as we go
  typename InputMeshType::PointsContainer::ConstPointer points = mesh->GetPoints();

  // In ITK the point container is not necessarily a vector, but in VTK it is
  vtkIdType VTKId = 0;
  //std::unordered_map< vtkIdType, SizeValueType > indexMap;
  for(typename InputMeshType::PointsContainer::ConstIterator pointsIt = points->Begin();
      pointsIt != points->End(); ++pointsIt, ++VTKId)
    {
    // Get the point index from the point container iterator
    //indexMap[ VTKId ] = pointIt->Index();

    // Set the vtk point at the index with the the coord array from itk
    // itk returns a const pointer, but vtk is not const correct, so
    // we have to use a const cast to get rid of the const
    // TODO: Convert float to double if MeshTraits are using double to store
    // the mesh points
    vpoints->SetPoint(VTKId, const_cast<float*>(pointsIt->Value().GetDataPointer()));
    }

  // Set the points on the vtk grid
  this->m_UnstructuredGrid->SetPoints(vpoints);

  // Setup some VTK things
  int vtkCellCount = 0; // running counter for current cell being inserted into vtk
  int numCells = mesh->GetNumberOfCells();
  int *types = new int[numCells]; // type array for vtk
  // create vtk cells and estimate the size
  vtkCellArray* cells = vtkCellArray::New();
  cells->EstimateSize(numCells, 4);

  using CellTraits = typename InputMeshType::CellTraits;
  using PixelType = typename InputMeshType::PixelType;

  // Setup the line visitor
  typedef CellInterfaceVisitorImplementation<
    PixelType, CellTraits,
    LineCell< CellInterface< PixelType, CellTraits > >,
    VisitVTKCellsClass< InputMeshType > > LineVisitor;
  typename LineVisitor::Pointer lineVisitor =  LineVisitor::New();
  lineVisitor->SetTypeArray(types);
  lineVisitor->SetCellCounter(&vtkCellCount);
  lineVisitor->SetCellArray(cells);

  // Setup the triangle visitor
  typedef CellInterfaceVisitorImplementation<
    PixelType, CellTraits,
    TriangleCell< CellInterface< PixelType, CellTraits > >,
    VisitVTKCellsClass< InputMeshType > > TriangleVisitor;
  typename TriangleVisitor::Pointer triangleVisitor = TriangleVisitor::New();
  triangleVisitor->SetTypeArray(types);
  triangleVisitor->SetCellCounter(&vtkCellCount);
  triangleVisitor->SetCellArray(cells);

  // Setup the quadrilateral visitor
  typedef CellInterfaceVisitorImplementation<
    PixelType, CellTraits,
    QuadrilateralCell< CellInterface< PixelType, CellTraits > >,
    VisitVTKCellsClass< InputMeshType > > QuadrilateralVisitor;
  typename QuadrilateralVisitor::Pointer quadrilateralVisitor =  QuadrilateralVisitor::New();
  quadrilateralVisitor->SetTypeArray(types);
  quadrilateralVisitor->SetCellCounter(&vtkCellCount);
  quadrilateralVisitor->SetCellArray(cells);

  // Setup the polygon visitor
  typedef CellInterfaceVisitorImplementation<
    PixelType, CellTraits,
    PolygonCell< CellInterface< PixelType, CellTraits > >,
    VisitVTKCellsClass< InputMeshType > > PolygonVisitor;
  typename PolygonVisitor::Pointer polygonVisitor =  PolygonVisitor::New();
  polygonVisitor->SetTypeArray(types);
  polygonVisitor->SetCellCounter(&vtkCellCount);
  polygonVisitor->SetCellArray(cells);

  // Setup the tetrahedron visitor
  typedef CellInterfaceVisitorImplementation<
    PixelType, CellTraits,
    TetrahedronCell< CellInterface< PixelType, CellTraits > >,
    VisitVTKCellsClass< InputMeshType > > TetrahedronVisitor;
  typename TetrahedronVisitor::Pointer tetrahedronVisitor =  TetrahedronVisitor::New();
  tetrahedronVisitor->SetTypeArray(types);
  tetrahedronVisitor->SetCellCounter(&vtkCellCount);
  tetrahedronVisitor->SetCellArray(cells);

  // Setup the hexahedron visitor
  typedef CellInterfaceVisitorImplementation<
    PixelType, CellTraits,
    HexahedronCell< CellInterface< PixelType, CellTraits > >,
    VisitVTKCellsClass< InputMeshType > > HexahedronVisitor;
  typename HexahedronVisitor::Pointer hexahedronVisitor =  HexahedronVisitor::New();
  hexahedronVisitor->SetTypeArray(types);
  hexahedronVisitor->SetCellCounter(&vtkCellCount);
  hexahedronVisitor->SetCellArray(cells);

  // Add the visitors to a multivisitor

  typename InputMeshType::CellType::MultiVisitor::Pointer multiVisitor = InputMeshType::CellType::MultiVisitor::New();

  multiVisitor->AddVisitor(lineVisitor.GetPointer());
  multiVisitor->AddVisitor(triangleVisitor.GetPointer());
  multiVisitor->AddVisitor(quadrilateralVisitor.GetPointer());
  multiVisitor->AddVisitor(polygonVisitor.GetPointer());
  multiVisitor->AddVisitor(tetrahedronVisitor.GetPointer());
  multiVisitor->AddVisitor(hexahedronVisitor.GetPointer());

  // Now ask the mesh to accept the multivisitor which
  // will Call Visit for each cell in the mesh that matches the
  // cell types of the visitors added to the MultiVisitor
  mesh->Accept(multiVisitor);

  // Now set the cells on the vtk grid with the type array and cell array
  this->m_UnstructuredGrid->SetCells(types, cells);

  // Clean up vtk objects
  delete [] types;
  cells->Delete();
  vpoints->Delete();
}

} // end namespace itk

#endif
