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
#ifndef itkFeatureAggregator_h
#define itkFeatureAggregator_h

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkProgressAccumulator.h"

namespace itk
{

/** \class FeatureAggregator
 * \brief Class for combining multiple features into a single one.
 *
 * This class is the base class for specific implementation of feature
 * mixing strategies.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT FeatureAggregator : public FeatureGenerator<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(FeatureAggregator);

  /** Standard class type alias. */
  using Self = FeatureAggregator;
  using Superclass = FeatureGenerator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** This is an abstract class, therefore it doesn't need the itkNewMacro() */

  /** Run-time type information (and related methods). */
  itkTypeMacro(FeatureAggregator, FeatureGenerator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = SpatialObject<NDimension>;
  using SpatialObjectPointer = typename SpatialObjectType::Pointer;
  using SpatialObjectConstPointer = typename SpatialObjectType::ConstPointer;

  /** Type of the image and specific SpatialObject produced as output */
  using OutputPixelType = float;
  using OutputImageType = Image<OutputPixelType, NDimension>;
  using OutputImageSpatialObjectType = ImageSpatialObject<NDimension, OutputPixelType>;

  /** Type of the class that will generate input features in the form of
   * spatial objects. */
  using FeatureGeneratorType = FeatureGenerator<Dimension>;

  /**
   * Method for adding a feature generator that will compute the Nth feature to
   * be passed to the segmentation module.
   */
  void
  AddFeatureGenerator(FeatureGeneratorType * generator);

  /** Check all feature generators and return consolidate MTime */
  ModifiedTimeType
  GetMTime() const override;

protected:
  FeatureAggregator();
  ~FeatureAggregator() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;

  unsigned int
  GetNumberOfInputFeatures() const;

  using InputFeatureType = typename FeatureGeneratorType::SpatialObjectType;

  const InputFeatureType *
  GetInputFeature(unsigned int featureId) const;

  ProgressAccumulator::Pointer m_ProgressAccumulator;

private:
  using FeatureGeneratorPointer = typename FeatureGeneratorType::Pointer;
  using FeatureGeneratorArrayType = std::vector<FeatureGeneratorPointer>;
  using FeatureGeneratorIterator = typename FeatureGeneratorArrayType::iterator;
  using FeatureGeneratorConstIterator = typename FeatureGeneratorArrayType::const_iterator;

  FeatureGeneratorArrayType m_FeatureGenerators;

  void
  UpdateAllFeatureGenerators();

  void virtual ConsolidateFeatures() = 0;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkFeatureAggregator.hxx"
#endif

#endif
