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
#ifndef itkDescoteauxSheetnessFeatureGenerator_h
#define itkDescoteauxSheetnessFeatureGenerator_h

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkSymmetricSecondRankTensor.h"
#include "itkSymmetricEigenAnalysisImageFilter.h"
#include "itkDescoteauxSheetnessImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

namespace itk
{

/** \class DescoteauxSheetnessFeatureGenerator
 * \brief Generates a feature image by computing a Sheetness measures based on
 * the Hessian Eigenvalues.
 *
 * This is based on the filter proposed by Descoteux et al.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT DescoteauxSheetnessFeatureGenerator : public FeatureGenerator<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(DescoteauxSheetnessFeatureGenerator);

  /** Standard class type alias. */
  using Self = DescoteauxSheetnessFeatureGenerator;
  using Superclass = FeatureGenerator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DescoteauxSheetnessFeatureGenerator, FeatureGenerator);

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

  /** Sigma value to be used in the Gaussian smoothing preceding the
   * Hessian computation. */
  itkSetMacro(Sigma, double);
  itkGetMacro(Sigma, double);

  /** Sheetness normalization value to be used in the Descoteaux sheetness filter. */
  itkSetMacro(SheetnessNormalization, double);
  itkGetMacro(SheetnessNormalization, double);

  /** Bloobiness normalization value to be used in the Descoteaux sheetness filter. */
  itkSetMacro(BloobinessNormalization, double);
  itkGetMacro(BloobinessNormalization, double);

  /** Noise normalization value to be used in the Descoteaux sheetness filter. */
  itkSetMacro(NoiseNormalization, double);
  itkGetMacro(NoiseNormalization, double);

  /** Defines whether the filter will look for Bright sheets over a Dark
   * background or for Dark sheets over a Bright background. */
  itkSetMacro(DetectBrightSheets, bool);
  itkGetMacro(DetectBrightSheets, bool);
  itkBooleanMacro(DetectBrightSheets);

protected:
  DescoteauxSheetnessFeatureGenerator();
  ~DescoteauxSheetnessFeatureGenerator() override;
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

  using HessianFilterType = HessianRecursiveGaussianImageFilter<InputImageType>;
  using HessianImageType = typename HessianFilterType::OutputImageType;
  using HessianPixelType = typename HessianImageType::PixelType;

  using EigenValueArrayType = FixedArray<double, HessianPixelType::Dimension>;
  using EigenValueImageType = Image<EigenValueArrayType, Dimension>;

  using EigenAnalysisFilterType = SymmetricEigenAnalysisImageFilter<HessianImageType, EigenValueImageType>;

  using SheetnessFilterType = DescoteauxSheetnessImageFilter<EigenValueImageType, OutputImageType>;

  using RescaleFilterType = RescaleIntensityImageFilter<OutputImageType, OutputImageType>;

  typename HessianFilterType::Pointer       m_HessianFilter;
  typename EigenAnalysisFilterType::Pointer m_EigenAnalysisFilter;
  typename SheetnessFilterType::Pointer     m_SheetnessFilter;
  typename RescaleFilterType::Pointer       m_RescaleFilter;

  double m_Sigma;
  double m_SheetnessNormalization;
  double m_BloobinessNormalization;
  double m_NoiseNormalization;
  bool   m_DetectBrightSheets;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkDescoteauxSheetnessFeatureGenerator.hxx"
#endif

#endif
