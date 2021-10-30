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
#ifndef itkBoxSpatialObjectToVTKPolyDataFilter_h
#define itkBoxSpatialObjectToVTKPolyDataFilter_h

#include "itkSpatialObjectToVTKPolyDataFilter.h"
#include "itkBoxSpatialObject.h"
#include "itkProcessObject.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"

namespace itk
{

/** \class BoxSpatialObjectToVTKPolyDataFilter
 * \brief Converts an ITK BoxSpatialObject into a VTK PolyData and plugs a
 *  itk data pipeline to a VTK datapipeline.
 *
 * \ingroup   ITKVtkGlue
 */
template< unsigned int VDimension >
class ITK_TEMPLATE_EXPORT BoxSpatialObjectToVTKPolyDataFilter : public SpatialObjectToVTKPolyDataFilter< VDimension >
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(BoxSpatialObjectToVTKPolyDataFilter);

  /** Standard class type aliases. */
  using Self = BoxSpatialObjectToVTKPolyDataFilter;
  using Superclass = SpatialObjectToVTKPolyDataFilter< VDimension >;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BoxSpatialObjectToVTKPolyDataFilter, SpatialObjectToVTKPolyDataFilter);

  /** Some type alias. */
  static constexpr unsigned int Dimension = VDimension;
  using SpatialObjectType = BoxSpatialObject< Dimension >;

  /** Set the input in the form of an itk::Mesh */
  using Superclass::SetInput;
  void SetInput( const SpatialObjectType * );
  const SpatialObjectType * GetInput() const;
  const SpatialObjectType * GetInput(unsigned int idx) const;

protected:
  BoxSpatialObjectToVTKPolyDataFilter();
  ~BoxSpatialObjectToVTKPolyDataFilter() override;

  void GenerateData() override;

private:
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBoxSpatialObjectToVTKPolyDataFilter.hxx"
#endif

#endif
