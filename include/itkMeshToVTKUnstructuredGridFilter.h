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
#ifndef itkMeshToVTKUnstructuredGridFilter_h
#define itkMeshToVTKUnstructuredGridFilter_h

#include "itkProcessObject.h"
#include "vtkUnstructuredGrid.h"
#include "vtkSmartPointer.h"

namespace itk
{

/** \class MeshToVTKUnstructuredGridFilter
 * \brief Converts an ITK Mesh or PointSet into a VTK UnstructuredGrid and plugs a
 *  itk data pipeline to a VTK datapipeline.
 *
 *  \todo: Transfer PointData and CellData
 *
 * \ingroup   ITKVtkGlue
 */
template< typename TInputMesh >
class ITK_TEMPLATE_EXPORT MeshToVTKUnstructuredGridFilter : public ProcessObject
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(MeshToVTKUnstructuredGridFilter);

  /** Standard class type aliases. */
  using Self = MeshToVTKUnstructuredGridFilter;
  using Superclass = ProcessObject;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MeshToVTKUnstructuredGridFilter, ProcessObject);

  /** Some type alias. */
  using InputMeshType = TInputMesh;

  /** Get the output in the form of a vtkUnstructuredGrid.  */
  vtkUnstructuredGrid * GetOutput();

  /** Set the input in the form of an itk::Mesh */
  using Superclass::SetInput;
  void SetInput( const InputMeshType * );
  const InputMeshType * GetInput() const;
  const InputMeshType * GetInput(unsigned int idx) const;

  void Update() override;

protected:
  MeshToVTKUnstructuredGridFilter();
  ~MeshToVTKUnstructuredGridFilter() override;

  void GenerateData() override;

  void PrintSelf(std::ostream & os, Indent indent) const override;

  vtkSmartPointer< vtkUnstructuredGrid > m_UnstructuredGrid;

  ModifiedTimeType m_ProcessedInputMTime{0};

private:
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMeshToVTKUnstructuredGridFilter.hxx"
#endif

#endif
