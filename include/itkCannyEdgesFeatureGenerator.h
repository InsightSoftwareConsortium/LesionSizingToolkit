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
#ifndef itkCannyEdgesFeatureGenerator_h
#define itkCannyEdgesFeatureGenerator_h

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkCastImageFilter.h"
#include "itkCannyEdgeDetectionRecursiveGaussianImageFilter.h"
#include "itkIntensityWindowingImageFilter.h"

namespace itk
{

/** \class CannyEdgesFeatureGenerator
 * \brief Generates a feature image by computing the inverse map to the canny
 * edges in the image.
 *
 * \par Overview
 * The class generates features that can be used as the speed term for
 * computing a canny level set. The class takes an input image
 *
 *    Input -> CastToFloat -> CannyEdgeFilter -> InvertFilter
 *
 * The resulting feature is ideally used as the speed term for a level set
 * segmentation module. The speed feature generated is designed to lock
 * onto edges (which are extracted by the canny filter).
 *
 * There are two parameters to this feature generator.
 * (1) UpperThreshold/LowerThreshold: These set the thresholding values of
 *     the Canny edge detection. The canny algorithm incorporates a
 *     hysteresis thresholding which is applied to the gradient magnitude
 *     of the smoothed image to find edges.
 * (2) Sigma.  Controls the smoothing parameter of the Gaussian filtering
 *     done during Canny edge detection. The first step of canny edge
 *     detection is to smooth the input with a Gaussian filter. Second
 *     derivatives etc are computed on the smoothed image.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT CannyEdgesFeatureGenerator : public FeatureGenerator<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(CannyEdgesFeatureGenerator);

  /** Standard class type alias. */
  using Self = CannyEdgesFeatureGenerator;
  using Superclass = FeatureGenerator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CannyEdgesFeatureGenerator, FeatureGenerator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input to this
   * feature generator. */
  using InputPixelType = signed short;
  using InputImageType = Image<InputPixelType, Dimension>;
  using InputImageSpatialObjectType = ImageSpatialObject<NDimension, InputPixelType>;
  using InputImageSpatialObjectPointer = typename InputImageSpatialObjectType::Pointer;
  using SpatialObjectType = typename Superclass::SpatialObjectType;

  using ScalarRealType = typename NumericTraits<InputPixelType>::ScalarRealType;
  using SigmaArrayType = FixedArray<ScalarRealType, itkGetStaticConstMacro(Dimension)>;

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

  /** Set Sigma value. Sigma is measured in the units of image spacing. You
    may use the method SetSigma to set the same value across each axis or
    use the method SetSigmaArray if you need different values along each
    axis. */
  void
  SetSigmaArray(const SigmaArrayType & sigmas);
  void
  SetSigma(ScalarRealType sigma);
  SigmaArrayType
  GetSigmaArray() const;
  ScalarRealType
  GetSigma() const;

  itkSetMacro(UpperThreshold, double);
  itkGetMacro(UpperThreshold, double);
  itkSetMacro(LowerThreshold, double);
  itkGetMacro(LowerThreshold, double);

protected:
  CannyEdgesFeatureGenerator();
  ~CannyEdgesFeatureGenerator() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;

private:
  using InternalPixelType = float;
  using InternalImageType = Image<InternalPixelType, Dimension>;

  using OutputPixelType = InternalPixelType;
  using OutputImageType = InternalImageType;

  using OutputImageSpatialObjectType = ImageSpatialObject<NDimension, OutputPixelType>;

  using CastFilterType = CastImageFilter<InputImageType, InternalImageType>;
  using CastFilterPointer = typename CastFilterType::Pointer;
  using CannyEdgeFilterType = CannyEdgeDetectionRecursiveGaussianImageFilter<InternalImageType, InternalImageType>;
  using CannyEdgeFilterPointer = typename CannyEdgeFilterType::Pointer;

  using RescaleFilterType = IntensityWindowingImageFilter<InternalImageType, InternalImageType>;
  using RescaleFilterPointer = typename RescaleFilterType::Pointer;

  CastFilterPointer      m_CastFilter;
  RescaleFilterPointer   m_RescaleFilter;
  CannyEdgeFilterPointer m_CannyFilter;

  double m_UpperThreshold;
  double m_LowerThreshold;

  /** Standard deviation of the gaussian used for smoothing */
  SigmaArrayType m_Sigma;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkCannyEdgesFeatureGenerator.hxx"
#endif

#endif
