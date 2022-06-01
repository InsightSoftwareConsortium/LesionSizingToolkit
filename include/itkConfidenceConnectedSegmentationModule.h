/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkConfidenceConnectedSegmentationModule_h
#define itkConfidenceConnectedSegmentationModule_h

#include "itkRegionGrowingSegmentationModule.h"
#include "itkConfidenceConnectedImageFilter.h"

namespace itk
{

/** \class ConfidenceConnectedSegmentationModule
 * \brief This class applies the connected threshold region growing
 * segmentation method.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT ConfidenceConnectedSegmentationModule : public RegionGrowingSegmentationModule<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(ConfidenceConnectedSegmentationModule);

  /** Standard class type alias. */
  using Self = ConfidenceConnectedSegmentationModule;
  using Superclass = RegionGrowingSegmentationModule<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConfidenceConnectedSegmentationModule, RegionGrowingSegmentationModule);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using FeatureImageType = typename Superclass::FeatureImageType;
  using OutputImageType = typename Superclass::OutputImageType;
  using InputSpatialObjectType = typename Superclass::InputSpatialObjectType;

  /** Factor that will be applied to the standard deviation in order to compute
   * the intensity range from which pixel will be included in the region. */
  itkSetMacro(SigmaMultiplier, double);
  itkGetMacro(SigmaMultiplier, double);

protected:
  ConfidenceConnectedSegmentationModule();
  ~ConfidenceConnectedSegmentationModule() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;

private:
  double m_SigmaMultiplier;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkConfidenceConnectedSegmentationModule.hxx"
#endif

#endif
