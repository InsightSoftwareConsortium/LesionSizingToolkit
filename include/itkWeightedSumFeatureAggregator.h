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
#ifndef itkWeightedSumFeatureAggregator_h
#define itkWeightedSumFeatureAggregator_h

#include "itkFeatureAggregator.h"

namespace itk
{

/** \class WeightedSumFeatureAggregator
 * \brief Class for combining multiple features into a single one by computing
 * a pixel-wise weighted sum that is normalized to produce a number between 0
 * and 1.
 *
 * This class generates a new feature object containing an image that is
 * computed as the pixel-wise weighted sum of all the input feature images.
 * The user must provide the weight to be used.
 *
 * \warning This class assumes that all the images have the same: origin,
 * spacing, orientation, and that they are represented in the same image grid.
 * mixing strategies.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT WeightedSumFeatureAggregator : public FeatureAggregator<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(WeightedSumFeatureAggregator);

  /** Standard class type alias. */
  using Self = WeightedSumFeatureAggregator;
  using Superclass = FeatureAggregator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(WeightedSumFeatureAggregator, FeatureAggregator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Add the weight that will be used to multiply the corresponding feature
   * when computing the weighted sum. */
  void virtual AddWeight(double weight);

  /** Type of the image and specific SpatialObject produced as output */
  using OutputPixelType = typename Superclass::OutputPixelType;
  using OutputImageType = typename Superclass::OutputImageType;
  using OutputImageSpatialObjectType = typename Superclass::OutputImageSpatialObjectType;

protected:
  WeightedSumFeatureAggregator();
  ~WeightedSumFeatureAggregator() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

private:
  void
  ConsolidateFeatures() override;

  using WeightsArrayType = std::vector<double>;

  WeightsArrayType m_Weights;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkWeightedSumFeatureAggregator.hxx"
#endif

#endif
