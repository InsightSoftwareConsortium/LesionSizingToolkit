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
#ifndef itkSatoVesselnessSigmoidFeatureGenerator_h
#define itkSatoVesselnessSigmoidFeatureGenerator_h

#include "itkSatoVesselnessFeatureGenerator.h"
#include "itkSigmoidImageFilter.h"

namespace itk
{

/** \class SatoVesselnessSigmoidFeatureGenerator
 * \brief Generates a feature image by computing the Sato Vesselness measure of the
 * input image and applying a sigmoid transformation to it.
 *
 * The typical use of this class would be to generate the Vessel-map needed by a
 * Level Set filter to internally compute its speed image.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT SatoVesselnessSigmoidFeatureGenerator : public SatoVesselnessFeatureGenerator<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(SatoVesselnessSigmoidFeatureGenerator);

  /** Standard class type alias. */
  using Self = SatoVesselnessSigmoidFeatureGenerator;
  using Superclass = SatoVesselnessFeatureGenerator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SatoVesselnessSigmoidFeatureGenerator, SatoVesselnessFeatureGenerator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Alpha value to be used in the Sigmoid transformation. It should be a
   * negative number if the feature is going to be used to exclude vessels, and
   * it should be a positive number if the feature is intended to include
   * vessels. */
  itkSetMacro(SigmoidAlpha, double);
  itkGetMacro(SigmoidAlpha, double);

  /** Beta value to be used in the Sigmoid transformation. It should be close
   * to the Vesselness value that is considered to be a threshold in Vesselness. */
  itkSetMacro(SigmoidBeta, double);
  itkGetMacro(SigmoidBeta, double);

protected:
  SatoVesselnessSigmoidFeatureGenerator();
  ~SatoVesselnessSigmoidFeatureGenerator() override;
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

  using SigmoidFilterType = SigmoidImageFilter<InternalImageType, InternalImageType>;

  typename SigmoidFilterType::Pointer m_SigmoidFilter;

  double m_SigmoidAlpha;
  double m_SigmoidBeta;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkSatoVesselnessSigmoidFeatureGenerator.hxx"
#endif

#endif
