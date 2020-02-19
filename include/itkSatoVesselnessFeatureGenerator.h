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
#ifndef itkSatoVesselnessFeatureGenerator_h
#define itkSatoVesselnessFeatureGenerator_h

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkHessian3DToVesselnessMeasureImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkSymmetricSecondRankTensor.h"
#include "itkVesselEnhancingDiffusion3DImageFilter.h"

namespace itk
{

/** \class SatoVesselnessFeatureGenerator
 * \brief Generates a feature image by computing the Sato Vesselness measure of
 * the input image.
 *
 * The typical use of this class would be to generate the Vesselness-map needed
 * by a Level Set filter to internally compute its speed image.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT SatoVesselnessFeatureGenerator : public FeatureGenerator<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(SatoVesselnessFeatureGenerator);

  /** Standard class type alias. */
  using Self = SatoVesselnessFeatureGenerator;
  using Superclass = FeatureGenerator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SatoVesselnessFeatureGenerator, FeatureGenerator);

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

  /** Alpha1 value to be used in the Sato Vesselness filter. */
  itkSetMacro(Alpha1, double);
  itkGetMacro(Alpha1, double);

  /** Alpha2 value to be used in the Sato Vesselness filter. */
  itkSetMacro(Alpha2, double);
  itkGetMacro(Alpha2, double);

  /** Use vessel enhancing diffusion ? Defaults to false. */
  itkSetMacro(UseVesselEnhancingDiffusion, bool);
  itkGetMacro(UseVesselEnhancingDiffusion, bool);
  itkBooleanMacro(UseVesselEnhancingDiffusion);

protected:
  SatoVesselnessFeatureGenerator();
  ~SatoVesselnessFeatureGenerator() override;
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
  using VesselnessMeasureFilterType = Hessian3DToVesselnessMeasureImageFilter<InternalPixelType>;
  using VesselEnhancingDiffusionFilterType = VesselEnhancingDiffusion3DImageFilter<InputPixelType, Dimension>;

  typename HessianFilterType::Pointer                  m_HessianFilter;
  typename VesselnessMeasureFilterType::Pointer        m_VesselnessFilter;
  typename VesselEnhancingDiffusionFilterType::Pointer m_VesselEnhancingDiffusionFilter;

  double m_Sigma;
  double m_Alpha1;
  double m_Alpha2;
  bool   m_UseVesselEnhancingDiffusion;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkSatoVesselnessFeatureGenerator.hxx"
#endif

#endif
