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
#ifndef itkMorphologicalOpeningFeatureGenerator_h
#define itkMorphologicalOpeningFeatureGenerator_h

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkVotingBinaryHoleFillFloodingImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "itkCastImageFilter.h"

namespace itk
{

/** \class MorphologicalOpeningFeatureGenerator
 * \brief Generates a feature image based on intensity and removes small pieces from it.
 *
 * This feature generator thresholds the input image, runs an Openning
 * Mathematical Morphology Filter and then a Voting Hole Filling filter.
 * The net effect is the elimination of small islands and small holes
 * from the thresholded image.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT MorphologicalOpeningFeatureGenerator : public FeatureGenerator<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(MorphologicalOpeningFeatureGenerator);

  /** Standard class type alias. */
  using Self = MorphologicalOpeningFeatureGenerator;
  using Superclass = FeatureGenerator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkOverrideGetNameOfClassMacro(MorphologicalOpeningFeatureGenerator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input to this
   * feature generator. */
  using InputPixelType = signed short;
  using InputImageType = Image<InputPixelType, Dimension>;
  using InputImageSpatialObjectType = ImageSpatialObject<NDimension, InputPixelType>;
  using InputImageSpatialObjectPointer = typename InputImageSpatialObjectType::Pointer;
  using SpatialObjectType = typename Superclass::SpatialObjectType;

  /** Input data that will be used for generating the feature. */
  using ProcessObject::SetInput;
  void
  SetInput(const SpatialObjectType * input);
  const SpatialObjectType *
  GetInput() const;

  /** Output data that carries the feature in the form of a
   * SpatialObject. */
  const SpatialObjectType *
  GetFeature() const;

  /** Set the Hounsfield Unit value to threshold the Lung. */
  itkSetMacro(LungThreshold, InputPixelType);
  itkGetMacro(LungThreshold, InputPixelType);

protected:
  MorphologicalOpeningFeatureGenerator();
  ~MorphologicalOpeningFeatureGenerator() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;

private:
  using InternalPixelType = unsigned char;
  using InternalImageType = Image<InternalPixelType, Dimension>;

  using OutputPixelType = float;
  using OutputImageType = Image<OutputPixelType, Dimension>;

  using OutputImageSpatialObjectType = ImageSpatialObject<NDimension, OutputPixelType>;

  using ThresholdFilterType = BinaryThresholdImageFilter<InputImageType, InternalImageType>;
  using ThresholdFilterPointer = typename ThresholdFilterType::Pointer;

  using KernelType = BinaryBallStructuringElement<InternalPixelType, Dimension>;
  using OpenningFilterType = BinaryMorphologicalOpeningImageFilter<InternalImageType, InternalImageType, KernelType>;
  using OpenningFilterPointer = typename OpenningFilterType::Pointer;

  using VotingHoleFillingFilterType = VotingBinaryHoleFillFloodingImageFilter<InternalImageType, InternalImageType>;
  using VotingHoleFillingFilterPointer = typename VotingHoleFillingFilterType::Pointer;

  using CastingFilterType = CastImageFilter<InternalImageType, OutputImageType>;
  using CastingFilterPointer = typename CastingFilterType::Pointer;


  ThresholdFilterPointer         m_ThresholdFilter;
  OpenningFilterPointer          m_OpenningFilter;
  VotingHoleFillingFilterPointer m_VotingHoleFillingFilter;
  CastingFilterPointer           m_CastingFilter;

  InputPixelType m_LungThreshold;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkMorphologicalOpeningFeatureGenerator.hxx"
#endif

#endif
