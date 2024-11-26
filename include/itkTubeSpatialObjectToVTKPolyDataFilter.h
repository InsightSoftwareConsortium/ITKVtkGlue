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
#ifndef itkTubeSpatialObjectToVTKPolyDataFilter_h
#define itkTubeSpatialObjectToVTKPolyDataFilter_h

#include "itkSpatialObjectToVTKPolyDataFilter.h"
#include "itkTubeSpatialObject.h"

namespace itk
{

/** \class TubeSpatialObjectToVTKPolyDataFilter
 * \brief Converts an ITK TubeSpatialObject into a VTK PolyData and plugs a
 *  itk data pipeline to a VTK datapipeline.
 *
 * \ingroup   ITKVtkGlue
 */
template< typename TTube >
class ITK_TEMPLATE_EXPORT TubeSpatialObjectToVTKPolyDataFilter : public SpatialObjectToVTKPolyDataFilter< TTube::SpatialObject::ObjectDimension >
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(TubeSpatialObjectToVTKPolyDataFilter);

  /** Standard class type aliases. */
  using Self = TubeSpatialObjectToVTKPolyDataFilter;
  using Superclass = SpatialObjectToVTKPolyDataFilter< TTube::SpatialObject::ObjectDimension >;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TubeSpatialObjectToVTKPolyDataFilter, SpatialObjectToVTKPolyDataFilter);

  /** Some type alias. */
  static constexpr unsigned int Dimension = TTube::Dimension;
  using SpatialObjectType = TTube;

  /** Set the input in the form of an itk::Mesh */
  using Superclass::SetInput;
  void SetInput( const SpatialObjectType * );
  const SpatialObjectType * GetInput() const;
  const SpatialObjectType * GetInput(unsigned int idx) const;

  /** Set/Get the number of sides on the tube. */
  itkSetMacro( NumberOfSides, unsigned int );
  itkGetConstMacro( NumberOfSides, unsigned int );

  /** Use colors defined on the TubeSpatialObjectPoint */
  itkBooleanMacro( UsePointColors);
  itkSetMacro( UsePointColors, bool );
  itkGetConstMacro( UsePointColors, bool );

protected:
  TubeSpatialObjectToVTKPolyDataFilter();
  ~TubeSpatialObjectToVTKPolyDataFilter() override;

  void GenerateData() override;

  unsigned int m_NumberOfSides{ 8 };
  bool m_UsePointColors{ false };

private:
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTubeSpatialObjectToVTKPolyDataFilter.hxx"
#endif

#endif
