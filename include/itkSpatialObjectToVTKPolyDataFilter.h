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
#ifndef itkSpatialObjectToVTKPolyDataFilter_h
#define itkSpatialObjectToVTKPolyDataFilter_h

#include "itkSpatialObject.h"
#include "itkProcessObject.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"

namespace itk
{

/** \class SpatialObjectToVTKPolyDataFilter
 * \brief A base class for filters that converts an ITK SpatialObject into
 * a VTK PolyData.
 *
 * \ingroup   ITKVtkGlue
 */
template< unsigned int VDimension >
class ITK_TEMPLATE_EXPORT SpatialObjectToVTKPolyDataFilter : public ProcessObject
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(SpatialObjectToVTKPolyDataFilter);

  /** Standard class type aliases. */
  using Self = SpatialObjectToVTKPolyDataFilter;
  using Superclass = ProcessObject;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SpatialObjectToVTKPolyDataFilter, ProcessObject);

  /** Some type alias. */
  static constexpr unsigned int Dimension = VDimension;
  using SpatialObjectType = SpatialObject< Dimension >;

  /** Get the output in the form of a vtkPolyData.  */
  vtkPolyData * GetOutput();

  /** Set the input in the form of an itk::Mesh */
  using Superclass::SetInput;
  void SetInput( const SpatialObjectType * );
  const SpatialObjectType * GetInput() const;
  const SpatialObjectType * GetInput(unsigned int idx) const;

  void Update() override;

protected:
  SpatialObjectToVTKPolyDataFilter();
  ~SpatialObjectToVTKPolyDataFilter() override;

  void ApplyObjectToWorldTransformToPolyData();

  void PrintSelf(std::ostream & os, Indent indent) const override;

  vtkSmartPointer< vtkPolyData > m_PolyData;

  ModifiedTimeType m_ProcessedInputMTime{0};

private:
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSpatialObjectToVTKPolyDataFilter.hxx"
#endif

#endif

