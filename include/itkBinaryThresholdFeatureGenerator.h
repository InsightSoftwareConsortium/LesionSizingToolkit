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
#ifndef itkBinaryThresholdFeatureGenerator_h
#define itkBinaryThresholdFeatureGenerator_h

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"

namespace itk
{

/** \class BinaryThresholdFeatureGenerator
 * \brief Generates a feature image by thresholding the input intensities.
 *
 * The typical use of this class would be to generate the feature image needed
 * by a Level Set filter to internally compute its speed image.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class BinaryThresholdFeatureGenerator : public FeatureGenerator<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(BinaryThresholdFeatureGenerator);

  /** Standard class type alias. */
  using Self = BinaryThresholdFeatureGenerator;
  using Superclass = FeatureGenerator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BinaryThresholdFeatureGenerator, FeatureGenerator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input to this
   * feature generator. */
  using InputPixelType = signed short;
  using InputImageType = Image<InputPixelType, Dimension>;
  using InputImageSpatialObjectType = ImageSpatialObject<NDimension, InputPixelType>;
  using InputImageSpatialObjectPointer = typename InputImageSpatialObjectType::Pointer;
  using SpatialObjectType = typename Superclass::SpatialObjectType;

  /** Value to be used to threshold the image. */
  itkSetMacro(Threshold, double);
  itkGetMacro(Threshold, double);

protected:
  BinaryThresholdFeatureGenerator();
  ~BinaryThresholdFeatureGenerator() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;

private:
  using OutputPixelType = float;
  using OutputImageType = Image<OutputPixelType, Dimension>;

  using OutputImageSpatialObjectType = ImageSpatialObject<NDimension, OutputPixelType>;

  using BinaryThresholdFilterType = BinaryThresholdImageFilter<InputImageType, OutputImageType>;
  using BinaryThresholdFilterPointer = typename BinaryThresholdFilterType::Pointer;

  BinaryThresholdFilterPointer m_BinaryThresholdFilter;

  double m_Threshold;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkBinaryThresholdFeatureGenerator.hxx"
#endif

#endif
